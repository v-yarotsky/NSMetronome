#include "metronome.h"
#include "portaudio.h"
#include "sndfile.h"
#include <stdexcept>
#include <QDebug>

int PaCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    (void) input; /* Prevent unused variable warning. */
    (void) timeInfo;
    (void) statusFlags;

    MetronomeState *state = (MetronomeState *)userData;
    float *out = (float*)output;

    long long framesPerBeat = SAMPLE_RATE * 60 * 4 / state->tempo / state->beatsPerBar;
    const Sample *sample;

    for (unsigned long i = 0; i < frameCount; i++) {
        sample = state->barBeatNumber == 0 ? state->accentSample : state->sample;
        if (state->pos < sample->framesCount) {
            *out++ = sample->frames[state->pos];
            *out++ = sample->frames[state->pos];
        } else {
            *out++ = 0.0;
            *out++ = 0.0;

            // To change subdivision while playing, we wait for the currently playing sample to end,
            // and then reset the beat playhead and start on an accent.
            if (state->resetting) {
                state->resetting = false;
                state->barBeatNumber = 0;
                state->pos = 0;
                continue;
            };
        }
        state->pos++;
        if (state->pos > framesPerBeat) {
            state->barBeatNumber = (state->barBeatNumber + 1 ) % state->beatsPerBar;
            state->pos = 0;
        }
    }
    return 0;
}

Metronome::Metronome(std::string samplePath, std::string accentSamplePath)
{
    const Sample *sample = loadSample(samplePath);
    const Sample *accentSample = loadSample(accentSamplePath);

    this->state = new MetronomeState { 80, 4, 0, sample, accentSample, 0, false };

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        qDebug() << "well shit, failed to initialize portaudio:" << Pa_GetErrorText(err);
    }

    err = Pa_OpenDefaultStream(&this->stream, 0, 2, paFloat32, SAMPLE_RATE, 256, PaCallback, this->state);
    if (err != paNoError) {
        qDebug() << "well shit, failed to open audio stream:" << Pa_GetErrorText(err);
    }
}

const Sample * Metronome::loadSample(std::string path) {
    SF_INFO i;
    SNDFILE *f = sf_open(path.c_str(), SFM_READ, &i);
    if (f == NULL) {
        qDebug() << "error reading sample" << path.c_str() << ": " << sf_strerror(NULL);
    }
    float *frames = (float *)malloc(sizeof(float) * i.frames);
    sf_readf_float(f, frames, i.frames);
    sf_close(f);
    if (i.samplerate != SAMPLE_RATE) {
        throw std::invalid_argument("Unsupported sample rate for sample");
    }
    return new Sample { i.frames, frames };
}

Metronome::~Metronome() {
    PaError err = Pa_Terminate();
    if (err != paNoError) {
        qDebug() << "well shit, failed to stop portaudio:" << Pa_GetErrorText(err);
    }
}

void Metronome::start() {
    PaError err = Pa_StartStream(this->stream);
    if (err != paNoError) {
        qDebug() << "well shit, failed to start stream:" << Pa_GetErrorText(err);
    } else {
        qDebug() << "started stream";
        this->state->barBeatNumber = 0;
        this->state->pos = 0;
        this->state->resetting = false;
    }
}

void Metronome::stop() {
    PaError err = Pa_StopStream(this->stream);
    if (err != paNoError) {
        qDebug() << "well shit, failed to stop stream:" << Pa_GetErrorText(err);
    } else {
        qDebug() << "stopped stream";
    }
}

uint Metronome::tempo() {
    return this->state->tempo;
}

void Metronome::setTempo(uint newTempo) {
    this->state->tempo = newTempo;
}

uint Metronome::beatsPerBar() {
    return this->state->beatsPerBar;
}

void Metronome::setBeatsPerBar(uint newBeatsPerBar) {
    this->state->resetting = true;
    this->state->beatsPerBar = newBeatsPerBar;
}

// Call this 4 times to set tempo
void Metronome::tapTempo() {
    auto now = std::chrono::system_clock::now();
    if (!m_tapTempoTimestamps.empty()) {
        auto latest = m_tapTempoTimestamps.front();
        if ((now - std::chrono::seconds(3)) > latest) { // clear out tap tempo buffer if the most
            m_tapTempoTimestamps.clear();               // recent tap was more than 3 seconds ago
        }
    }
    m_tapTempoTimestamps.push_front(now);
    if (m_tapTempoTimestamps.size() > 4) {
        m_tapTempoTimestamps.pop_back();
    }
    if (m_tapTempoTimestamps.size() == 4) {
        auto intervalBetweenFirstAndLastTap = m_tapTempoTimestamps.front() - m_tapTempoTimestamps.back();
        unsigned long averageInterval =
                std::chrono::duration_cast<std::chrono::milliseconds>(intervalBetweenFirstAndLastTap).count() / 3;
        unsigned int newTempo = 60000 / averageInterval;
        setTempo(newTempo);
    }
}

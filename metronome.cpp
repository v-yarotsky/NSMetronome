#include "metronome.h"
#include "portaudio.h"
#include "sndfile.h"
#include <QDebug>

int PaCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    (void) input; /* Prevent unused variable warning. */
    (void) timeInfo;
    (void) statusFlags;

    MetronomeState *state = (MetronomeState *)userData;
    float *out = (float*)output;
    long long framesPerBeat = state->sampleRate * 60 * 4 / state->tempo / state->beatsPerBar;

    for (unsigned long i = 0; i < frameCount; i++) {
        if (state->pos < state->framesCount) {
            *out++ = state->samples[state->pos];
            *out++ = state->samples[state->pos];
        } else {
            *out++ = 0.0;
            *out++ = 0.0;
        }
        state->pos++;
        if (state->pos > framesPerBeat) {
            state->pos = 0;
        }
    }
    return 0;
}

Metronome::Metronome(const char *samplePath)
{
    SF_INFO i;
    SNDFILE *f = sf_open(samplePath, SFM_READ, &i);
    if (f == NULL) {
        qDebug() << "error reading sample:" << sf_strerror(NULL);
    }
    float *samples = (float *)malloc(sizeof(float) * i.frames);
    sf_readf_float(f, samples, i.frames);
    sf_close(f);

    this->state = new MetronomeState { 80, 4, i.samplerate, i.frames, samples, 0 };

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        qDebug() << "well shit, failed to initialize portaudio:" << Pa_GetErrorText(err);
    }

    err = Pa_OpenDefaultStream(&this->stream, 0, 2, paFloat32, 44100, 256, PaCallback, this->state);
    if (err != paNoError) {
        qDebug() << "well shit, failed to open audio stream:" << Pa_GetErrorText(err);
    }
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
    }
}

void Metronome::stop() {
    PaError err = Pa_StopStream(this->stream);
    if (err != paNoError) {
        qDebug() << "well shit, failed to stop stream:" << Pa_GetErrorText(err);
    } else {
        qDebug() << "stopped stream";
        this->state->pos = 0;
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
    this->state->beatsPerBar = newBeatsPerBar;
}

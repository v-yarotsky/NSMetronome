#include "metronomewindow.h"
#include "ui_metronomewindow.h"
#include "portaudio.h"
#include <stdlib.h>

int PaCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    (void) input; /* Prevent unused variable warning. */

    MetronomeState *state = (MetronomeState *)userData;
    float *out = (float*)output;
    int64_t framesPerBeat = state->sampleRate * 60 / state->tempo;

    for (uint i = 0; i < frameCount; i++) {
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

MetronomeWindow::MetronomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MetronomeWindow)
{
    SF_INFO i;
    SNDFILE *f = sf_open("./sounds/Metronome.wav", SFM_READ, &i);
    if (f == NULL) {
        printf("error reading sample: %s\n", sf_strerror (NULL));
    }
    printf("sample rate: %d\n", i.samplerate);
    printf("frames count: %d\n", i.frames);
    printf("channels count: %d\n", i.channels);
    float *samples = (float *)malloc(sizeof(float) * i.frames);
    sf_readf_float(f, samples, i.frames);
    sf_close(f);
    //mFile.close();

    this->state = new MetronomeState { 80, i.samplerate, i.frames, samples, 0 };

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        printf("well shit, failed to initialize portaudio: %s\n", Pa_GetErrorText(err));
    }

    err = Pa_OpenDefaultStream(&this->stream, 0, 2, paFloat32, 44100, 256, PaCallback, this->state);
    if (err != paNoError) {
        printf("well shit, failed to open audio stream: %s\n", Pa_GetErrorText(err));
    }

    ui->setupUi(this);
}

MetronomeWindow::~MetronomeWindow()
{
    delete ui;

    PaError err = Pa_Terminate();
    if (err != paNoError) {
        printf("well shit, failed to stop portaudio: %s\n", Pa_GetErrorText(err));
    }
}

void MetronomeWindow::startStopMetronome(bool start) {
    if (start) {
        PaError err = Pa_StartStream(this->stream);
        if (err != paNoError) {
            printf("well shit, failed to start stream: %s\n", Pa_GetErrorText(err));
        } else {
            printf("started stream\n");
        }
    } else {
        PaError err = Pa_StopStream(this->stream);
        if (err != paNoError) {
            printf("well shit, failed to stop stream: %s\n", Pa_GetErrorText(err));
        } else {
            printf("started stream\n");
        }
        this->state->pos = 0;
    }
}

void MetronomeWindow::changeTempo(int newTempo) {
    this->state->tempo = newTempo;
}

#include "metronomewindow.h"
#include "ui_metronomewindow.h"
#include "portaudio.h"
#include "sndfile.h"
#include <QDebug>

int PaCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    (void) input; /* Prevent unused variable warning. */
    (void) timeInfo;
    (void) statusFlags;

    MetronomeState *state = (MetronomeState *)userData;
    float *out = (float*)output;
    int64_t framesPerBeat = state->sampleRate * 60 / state->tempo / (state->beatsPerBar / 4);

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

MetronomeWindow::MetronomeWindow(QString rcRoot, QWidget *parent) :
    m_rcRoot(rcRoot),
    QMainWindow(parent),
    ui(new Ui::MetronomeWindow)
{
    SF_INFO i;
    const char * samplePath = (m_rcRoot + "/sounds/Metronome.wav").toUtf8().constData();
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

    ui->setupUi(this);
    ui->btnQuarterth->setBeatsPerBar(4);
    ui->btnQuarterthTriplets->setBeatsPerBar(6);
    ui->btnEighth->setBeatsPerBar(8);
    ui->btnEighthTriplets->setBeatsPerBar(12);
    ui->btnSixteenth->setBeatsPerBar(16);
    ui->btnSixteenthTriplets->setBeatsPerBar(24);
}

MetronomeWindow::~MetronomeWindow()
{
    delete ui;

    PaError err = Pa_Terminate();
    if (err != paNoError) {
        qDebug() << "well shit, failed to stop portaudio:" << Pa_GetErrorText(err);
    }
}

void MetronomeWindow::startStopMetronome(bool start) {
    QPushButton *b = qobject_cast<QPushButton *>(sender());

    if (start) {
        PaError err = Pa_StartStream(this->stream);
        if (err != paNoError) {
            qDebug() << "well shit, failed to start stream:" << Pa_GetErrorText(err);
        } else {
            qDebug() << "started stream";
        }
        b->setText("Stop");
    } else {
        PaError err = Pa_StopStream(this->stream);
        if (err != paNoError) {
            qDebug() << "well shit, failed to stop stream:" << Pa_GetErrorText(err);
        } else {
            qDebug() << "stopped stream";
        }
        this->state->pos = 0;
        b->setText("Start");
    }
}

void MetronomeWindow::changeTempo(int newTempo) {
    this->state->tempo = newTempo;
}

void MetronomeWindow::chooseSubdivision(QAbstractButton *btn, bool toggled) {
    if (!toggled) {
        return;
    }
    QSubdivisionButton *b = qobject_cast<QSubdivisionButton *>(btn);
    this->state->beatsPerBar = b->beatsPerBar();
}

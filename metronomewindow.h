#ifndef METRONOMEWINDOW_H
#define METRONOMEWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "portaudio.h"
#include "sndfile.h"

namespace Ui {
class MetronomeWindow;
}

struct MetronomeState {
  uint tempo;
  int64_t sampleRate;
  int64_t framesCount;
  float *samples;
  int pos;
};

class MetronomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetronomeWindow(QWidget *parent = 0);
    ~MetronomeWindow();

public slots:
    void startStopMetronome(bool start);
    void changeTempo(int newTempo);

private:
    Ui::MetronomeWindow *ui;
    PaStream *stream;
    MetronomeState *state;
};

#endif // METRONOMEWINDOW_H

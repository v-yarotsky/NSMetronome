#ifndef METRONOMEWINDOW_H
#define METRONOMEWINDOW_H

#include <QMainWindow>
#include "portaudio.h"
#include "qsubdivisionbutton.h"

namespace Ui {
class MetronomeWindow;
}

struct MetronomeState {
  uint tempo;
  uint beatsPerBar;
  int64_t sampleRate;
  int64_t framesCount;
  float *samples;
  int pos;
};

class MetronomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetronomeWindow(QString rcRoot, QWidget *parent = 0);
    ~MetronomeWindow();

public slots:
    void startStopMetronome(bool start);
    void changeTempo(int newTempo);
    void chooseSubdivision(QAbstractButton *btn, bool toggled);

private:
    QString m_rcRoot;
    Ui::MetronomeWindow *ui;
    PaStream *stream;
    MetronomeState *state;
};

#endif // METRONOMEWINDOW_H

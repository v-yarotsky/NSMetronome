#ifndef METRONOMEWINDOW_H
#define METRONOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class MetronomeWindow;
}

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
};

#endif // METRONOMEWINDOW_H

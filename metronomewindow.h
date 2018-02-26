#ifndef METRONOMEWINDOW_H
#define METRONOMEWINDOW_H

#include <QMainWindow>
#include "metronome.h"
#include "qsubdivisionbutton.h"

namespace Ui {
class MetronomeWindow;
}

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
    void tapTempo();

private:
    QString m_rcRoot;
    Ui::MetronomeWindow *ui;
    Metronome *m_metronome;
};

#endif // METRONOMEWINDOW_H

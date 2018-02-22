#include "metronomewindow.h"
#include "ui_metronomewindow.h"

MetronomeWindow::MetronomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MetronomeWindow)
{
    ui->setupUi(this);
}

MetronomeWindow::~MetronomeWindow()
{
    delete ui;
}

void MetronomeWindow::startStopMetronome(bool start) {

}

void MetronomeWindow::changeTempo(int newTempo) {

}

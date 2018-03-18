#include "metronomewindow.h"
#include "ui_metronomewindow.h"
#include <string>

MetronomeWindow::MetronomeWindow(QString rcRoot, QWidget *parent) :
    QMainWindow(parent),
    m_rcRoot(rcRoot),
    ui(new Ui::MetronomeWindow)
{
    std::string samplePath = (m_rcRoot + "/sounds/Metronome.wav").toUtf8().constData();
    std::string accentSamplePath = (m_rcRoot + "/sounds/MetronomeUp.wav").toUtf8().constData();

    m_metronome = new Metronome(samplePath, accentSamplePath);

    ui->setupUi(this);
    ui->btnQuarterth->setBeatsPerBar(4);
    ui->btnQuarterthTriplets->setBeatsPerBar(6);
    ui->btnEighth->setBeatsPerBar(8);
    ui->btnEighthTriplets->setBeatsPerBar(12);
    ui->btnSixteenth->setBeatsPerBar(16);
    ui->btnSixteenthTriplets->setBeatsPerBar(24);

    switch (m_metronome->beatsPerBar()) {
        case 4: ui->btnQuarterth->setChecked(true); break;
        case 6: ui->btnQuarterthTriplets->setChecked(true); break;
        case 8: ui->btnEighth->setChecked(true); break;
        case 12: ui->btnEighthTriplets->setChecked(true); break;
        case 16: ui->btnSixteenth->setChecked(true); break;
        case 24: ui->btnSixteenthTriplets->setChecked(true); break;
    }

    ui->bpmSpinBox->setValue(m_metronome->tempo());
    ui->bpmSpinBox->setMinimum(MIN_TEMPO);
    ui->bpmSpinBox->setMaximum(MAX_TEMPO);
}

MetronomeWindow::~MetronomeWindow()
{
    delete m_metronome;
    delete ui;
}

void MetronomeWindow::startStopMetronome(bool start) {
    QPushButton *b = qobject_cast<QPushButton *>(sender());

    if (start) {
        m_metronome->start();
        b->setText("Stop");
    } else {
        m_metronome->stop();
        b->setText("Start");
    }
}

void MetronomeWindow::changeTempo(int newTempo) {
    if (newTempo < MIN_TEMPO) {
        newTempo = MIN_TEMPO;
    } else if (newTempo > MAX_TEMPO) {
        newTempo = MAX_TEMPO;
    }

    if (m_metronome->tempo() != newTempo) {
        m_metronome->setTempo(newTempo);
        emit tempoChanged(newTempo);
    }
}

void MetronomeWindow::chooseSubdivision(QAbstractButton *btn, bool toggled) {
    if (!toggled) {
        return;
    }
    QSubdivisionButton *b = qobject_cast<QSubdivisionButton *>(btn);
    m_metronome->setBeatsPerBar(b->beatsPerBar());
}

void MetronomeWindow::tapTempo() {
    m_metronome->tapTempo();
    ui->bpmSpinBox->setValue(m_metronome->tempo());
}

void MetronomeWindow::halfTempo() {
    auto currentTempo = m_metronome->tempo();
    changeTempo(currentTempo / 2);
}

void MetronomeWindow::doubleTempo() {
    auto currentTempo = m_metronome->tempo();
    changeTempo(currentTempo * 2);
}

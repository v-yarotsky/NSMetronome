#include "metronomewindow.h"
#include "ui_metronomewindow.h"

MetronomeWindow::MetronomeWindow(QString rcRoot, QWidget *parent) :
    QMainWindow(parent),
    m_rcRoot(rcRoot),
    ui(new Ui::MetronomeWindow)
{
    m_metronome = new Metronome((m_rcRoot + "/sounds/Metronome.wav").toUtf8().constData());

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
    m_metronome->setTempo(newTempo);
}

void MetronomeWindow::chooseSubdivision(QAbstractButton *btn, bool toggled) {
    if (!toggled) {
        return;
    }
    QSubdivisionButton *b = qobject_cast<QSubdivisionButton *>(btn);
    m_metronome->setBeatsPerBar(b->beatsPerBar());
}

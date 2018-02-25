#ifndef QSUBDIVISIONBUTTON_H
#define QSUBDIVISIONBUTTON_H

#include <QPushButton>

namespace Ui {
class QSubdivisionButton;
}

class QSubdivisionButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int beatsPerBar READ beatsPerBar WRITE setBeatsPerBar USER true)

public:

    QSubdivisionButton(QWidget *parent = 0) : QPushButton(parent) {}
    ~QSubdivisionButton() {}

    int beatsPerBar() const {
        return m_beatsPerBar;
    }

    void setBeatsPerBar(int beatsPerBar) {
        m_beatsPerBar = beatsPerBar;
    }

private:

    int m_beatsPerBar = 4;
};
#endif // QSUBDIVISIONBUTTON_H

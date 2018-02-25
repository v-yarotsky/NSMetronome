#include "metronomewindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString rcRoot = QFileInfo(argv[0]).dir().path();
    MetronomeWindow w(rcRoot);
    w.show();

    return a.exec();
}

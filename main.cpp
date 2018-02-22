#include "metronomewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MetronomeWindow w;
    w.show();

    return a.exec();
}

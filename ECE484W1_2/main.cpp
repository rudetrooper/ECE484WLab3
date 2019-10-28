#include "brightncontrast.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrightNContrast w;
    w.show();
    //QByteArray ba("0123");
    //w.broadcastSliders(ba);

    return a.exec();
}

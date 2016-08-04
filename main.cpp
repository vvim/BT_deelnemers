#include "buurtijd_deelnemers.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Buurtijd_deelnemers w;
    w.show();

    return a.exec();
}

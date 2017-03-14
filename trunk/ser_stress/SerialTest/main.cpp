#include <QCoreApplication>
#include <QCommandLineParser>

#include "serialapp.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    SerialApp sa("/dev/ttyADV3", 921600);

    return app.exec();
}

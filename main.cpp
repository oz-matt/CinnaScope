#include "mainwindow.h"
#include "signaldata.h"
#include "samplingthread.h"
#include "oscwidget.h"
#include <QApplication>
#include <QThread>
#include "cinna.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(800, 400);

    w.show();

    bool ok = app.exec();

    return ok;
}

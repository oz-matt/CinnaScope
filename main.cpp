#include "mainwindow.h"
#include "signaldata.h"
#include "samplingthread.h"
#include "oscwidget.h"
#include "cinnapcieinterface.h"
#include <QApplication>
#include <QThread>

CinnaPcieInterface cpi;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(800, 400);

    w.show();

    cpi.start();

    bool ok = app.exec();

    return ok;
}

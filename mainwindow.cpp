#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include "knob.h"
#include "wheelbox.h"
#include "samplingthread.h"
#include "oscwidget.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OscWidget *ow = new OscWidget();

    SamplingThread *samplingThread= new SamplingThread();
    samplingThread->setFrequency( ow->frequency() );
    samplingThread->setAmplitude( ow->amplitude() );

    // sample() is now called every 10ms.
    samplingThread->setInterval(3);

    ow->connect( ow, SIGNAL( frequencyChanged( double ) ),
        samplingThread, SLOT( setFrequency( double ) ) );
    ow->connect( ow, SIGNAL( amplitudeChanged( double ) ),
        samplingThread, SLOT( setAmplitude( double ) ) );
    ow->connect( ow, SIGNAL( signalIntervalChanged( double ) ),
        samplingThread, SLOT( setInterval( double ) ) );


    ow->start();

    samplingThread->start();

    setCentralWidget(ow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

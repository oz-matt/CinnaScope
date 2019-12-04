#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plot = new Plot(this);
    setCentralWidget(plot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

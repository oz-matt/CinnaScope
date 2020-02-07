#include "oscwidget.h"
#include "mainwindow.h"
#include "plot.h"
#include "knob.h"
#include "wheelbox.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qlineedit.h>
#include "cinnafixedknob.h"
#include "cinnastate.h"
#include <qevent.h>
#include <qwt_point_data.h>
#include <qwt_plot_curve.h>
#include "triggerline.h"

extern CinnaState cstate;

OscWidget::OscWidget( QWidget *parent ):
    QWidget( parent )
{
    //const double intervalLength = 10.0; // seconds

    d_plot = new Plot( this );
    d_plot->setIntervalLength( 0.5 );

    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, d_plot, &Plot::updateMe);

    d_timeperdivKnob = new Knob( "Time/div", 0, 10, this );
    d_timeperdivKnob->setValue( 5 );


    d_vperdivKnob = new Knob( "V/div", 0, 10, this );
    d_vperdivKnob->setValue( 5 );

    xline = new TriggerLine();

    double x[3] ={1.0, 0.0, 1.0};
        double y[3] ={1.0,0,0};

        xline->setData(new QwtCPointerData(x,y,3));
        xline->setPen(QPen(QColor(Qt::white),1,Qt::SolidLine));
        //xline->attach(d_plot);

    //d_intervalWheel = new WheelBox( "Displayed [s]", 0.1, 1.0, 0.01, this );
    //d_intervalWheel->setValue( 1.0 );

    //d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    //d_timerWheel->setValue( 10.0 );

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    //vLayout1->addWidget( d_intervalWheel );
    //vLayout1->addWidget( d_timerWheel );
    //vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_timeperdivKnob );
    vLayout1->addWidget( d_vperdivKnob );

    tpd_label = new QLabel(cstate.getTimePerDivString());
    vpd_label = new QLabel(cstate.getVPerDivString());

    tpd_label->setAlignment(Qt::AlignRight);
    vpd_label->setAlignment(Qt::AlignLeft);

    QHBoxLayout *label_layout = new QHBoxLayout(  );
    label_layout->addWidget(vpd_label);
    label_layout->addWidget(tpd_label);

    QHBoxLayout *layout3 = new QHBoxLayout(  );
    layout3->addWidget( d_plot, 10 );

    QVBoxLayout* vLayout2 = new QVBoxLayout();
    vLayout2->addLayout(layout3);
    vLayout2->addLayout(label_layout);

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addLayout(vLayout2, 1);
    layout->addLayout(vLayout1, 0);

    //connect( d_amplitudeKnob, SIGNAL( valueChanged( double ) ),
        //SIGNAL( amplitudeChanged( double ) ) );

    connect( d_timeperdivKnob, SIGNAL( wheelEvent(QWheelEvent*) ),
            this, SLOT( updateTimePerDivTextFromScroll(QWheelEvent*) ) );
    connect( d_timeperdivKnob, SIGNAL( StartMouseDragListen(QMouseEvent*) ),
            this, SLOT( StartMouseDragListen(QMouseEvent*) ) );
    connect( d_timeperdivKnob, SIGNAL( ContinueMouseDragListen(QMouseEvent*) ),
            this, SLOT( ContinueMouseDragListen(QMouseEvent*) ) );

    connect( d_vperdivKnob, SIGNAL( wheelEvent(QWheelEvent*) ),
            this, SLOT( updateTimePerDivTextFromScroll_vpd(QWheelEvent*) ) );
    connect( d_vperdivKnob, SIGNAL( StartMouseDragListen(QMouseEvent*) ),
            this, SLOT( StartMouseDragListen_vpd(QMouseEvent*) ) );
    connect( d_vperdivKnob, SIGNAL( ContinueMouseDragListen(QMouseEvent*) ),
            this, SLOT( ContinueMouseDragListen_vpd(QMouseEvent*) ) );

    //connect( d_frequencyKnob, SIGNAL( valueChanged( double ) ),
        //SIGNAL( frequencyChanged( double ) ) );
    //connect( d_timerWheel, SIGNAL( valueChanged( double ) ),
        //SIGNAL( signalIntervalChanged( double ) ) );

    //connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        //d_plot, SLOT( setIntervalLength( double ) ) );
}

void OscWidget::start()
{
    d_plot->start();
    timer->start();
}

double OscWidget::frequency() const
{
    return 1.0 ;//d_frequencyKnob->value();
}

double OscWidget::amplitude() const
{
    return d_timeperdivKnob->value();
}

double OscWidget::signalInterval() const
{
    return 1.0 ;//d_timerWheel->value();
}

double posknob = 5;
double posVknob = 5;

void OscWidget::incrementTimePerDiv()
{
    posknob += 1;
    if (posknob >= 10) posknob = 0;
    d_timeperdivKnob->setValue(posknob);
    cstate.incrementTimePerDiv();
    d_plot->setIntervalLength(cstate.getTimePerDivInterval());
    tpd_label->setText(cstate.getTimePerDivString());
}

void OscWidget::decrementTimePerDiv()
{
    posknob -= 1;
    if (posknob < 0) posknob = 9;
    d_timeperdivKnob->setValue(posknob);
    cstate.decrementTimePerDiv();
    d_plot->setIntervalLength(cstate.getTimePerDivInterval());
    tpd_label->setText(cstate.getTimePerDivString());
}
void OscWidget::incrementVPerDiv()
{

    posVknob += 1;
    if (posVknob >= 10) posVknob = 0;
    d_vperdivKnob->setValue(posVknob);
    cstate.incrementVPerDiv();
    d_plot->setAxisScale(QwtPlot::yLeft, cstate.getVPerDivNum() * -4.0, cstate.getVPerDivNum() * 4.0);
    vpd_label->setText(cstate.getVPerDivString());

xline->attach(d_plot);

    d_plot->replot();
}

void OscWidget::decrementVPerDiv()
{
    posVknob -= 1;
    if (posVknob < 0) posVknob = 9;
    d_vperdivKnob->setValue(posVknob);
    cstate.decrementVPerDiv();
    d_plot->setAxisScale(QwtPlot::yLeft, cstate.getVPerDivNum() * -4.0, cstate.getVPerDivNum() * 4.0);
    vpd_label->setText(cstate.getVPerDivString());
    xline->detach();
    d_plot->replot();
}

void OscWidget::updateTimePerDivTextFromScroll(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (numDegrees.y()  > 0)
    {
        this->incrementTimePerDiv();
    }
    else
    {
        this->decrementTimePerDiv();
    }
}

void OscWidget::updateTimePerDivTextFromScroll_vpd(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (numDegrees.y()  > 0)
    {
        this->incrementVPerDiv();
    }
    else
    {
        this->decrementVPerDiv();
    }
}


signed int mpos = 0;

void OscWidget::StartMouseDragListen(QMouseEvent *event)
{
    mpos = event->y();
}

void OscWidget::ContinueMouseDragListen(QMouseEvent *event)
{
    int dragSensitivity = 10;
    if (event->y() > (mpos + dragSensitivity))
    {
        this->decrementTimePerDiv();
        mpos = event->y();
    }
    else if (event->y() < (mpos - dragSensitivity))
    {
        this->incrementTimePerDiv();
        mpos = event->y();
    }
}

signed int mposv = 0;

void OscWidget::StartMouseDragListen_vpd(QMouseEvent *event)
{
    mposv = event->y();
}

void OscWidget::ContinueMouseDragListen_vpd(QMouseEvent *event)
{
    int dragSensitivity = 10;
    if (event->y() > (mposv + dragSensitivity))
    {
        this->decrementVPerDiv();
        mposv = event->y();
    }
    else if (event->y() < (mposv - dragSensitivity))
    {
        this->incrementVPerDiv();
        mposv = event->y();
    }
}

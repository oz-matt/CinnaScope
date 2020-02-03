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

    d_amplitudeKnob = new Knob( "Time/div", 0, 10, this );
    d_amplitudeKnob->setValue( 5 );

    //d_frequencyKnob = new Knob( "Frequency [Hz]", 0.1, 20.0, this );
    //d_frequencyKnob->setValue( 17.8 );

    //d_intervalWheel = new WheelBox( "Displayed [s]", 0.1, 1.0, 0.01, this );
    //d_intervalWheel->setValue( 1.0 );

    //d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    //d_timerWheel->setValue( 10.0 );

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    //vLayout1->addWidget( d_intervalWheel );
    //vLayout1->addWidget( d_timerWheel );
    //vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_amplitudeKnob );
    //vLayout1->addWidget( d_frequencyKnob );

    le = new QLabel(cstate.getTimePerDivString());

    le->setAlignment(Qt::AlignRight);

    QVBoxLayout* vLayout2 = new QVBoxLayout();
    vLayout2->addWidget( d_plot, 10 );
    vLayout2->addWidget(le);

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addLayout(vLayout2, 1);
    layout->addLayout(vLayout1, 0);

    //connect( d_amplitudeKnob, SIGNAL( valueChanged( double ) ),
        //SIGNAL( amplitudeChanged( double ) ) );


    connect( d_amplitudeKnob, SIGNAL( wheelEvent(QWheelEvent*) ),
            this, SLOT( updateTimePerDivTextFromScroll(QWheelEvent*) ) );
    connect( d_amplitudeKnob, SIGNAL( StartMouseDragListen(QMouseEvent*) ),
            this, SLOT( StartMouseDragListen(QMouseEvent*) ) );
    connect( d_amplitudeKnob, SIGNAL( ContinueMouseDragListen(QMouseEvent*) ),
            this, SLOT( ContinueMouseDragListen(QMouseEvent*) ) );
    connect( d_amplitudeKnob, SIGNAL( StopMouseDragListen(QMouseEvent*) ),
            this, SLOT( StopMouseDragListen(QMouseEvent*) ) );

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
    return d_amplitudeKnob->value();
}

double OscWidget::signalInterval() const
{
    return 1.0 ;//d_timerWheel->value();
}

double posknob = 5;

void OscWidget::incrementTimePerDiv()
{
    posknob += 1;
    if (posknob >= 10) posknob = 0;
    d_amplitudeKnob->setValue(posknob);
    cstate.incrementTimePerDiv();
    d_plot->setIntervalLength(cstate.getTimePerDivInterval());
    le->setText(cstate.getTimePerDivString());
}

void OscWidget::decrementTimePerDiv()
{
    posknob -= 1;
    if (posknob < 0) posknob = 9;
    d_amplitudeKnob->setValue(posknob);
    cstate.decrementTimePerDiv();
    d_plot->setIntervalLength(cstate.getTimePerDivInterval());
    le->setText(cstate.getTimePerDivString());
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

signed int mpos = 0;

void OscWidget::StartMouseDragListen(QMouseEvent *event)
{
    mpos = event->y();
}

void OscWidget::ContinueMouseDragListen(QMouseEvent *event)
{
    int dragSensitivity = 10;
    qDebug("y is: %d, x is: %d", event->y(), (mpos + dragSensitivity));
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

void OscWidget::StopMouseDragListen(QMouseEvent *event)
{
    mpos = 0;
}

#include "oscwidget.h"
#include "mainwindow.h"
#include "plot.h"
#include "knob.h"
#include "wheelbox.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtimer.h>

OscWidget::OscWidget( QWidget *parent ):
    QWidget( parent )
{
    //const double intervalLength = 10.0; // seconds

    d_plot = new Plot( this );
    d_plot->setIntervalLength( 0.5 );

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, d_plot, &Plot::updateMe);

    d_amplitudeKnob = new Knob( "Amplitude", 0.0, 200.0, this );
    d_amplitudeKnob->setValue( 160.0 );

    d_frequencyKnob = new Knob( "Frequency [Hz]", 0.1, 20.0, this );
    d_frequencyKnob->setValue( 17.8 );

    d_intervalWheel = new WheelBox( "Displayed [s]", 0.1, 1.0, 0.01, this );
    d_intervalWheel->setValue( 1.0 );

    d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    d_timerWheel->setValue( 10.0 );

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget( d_intervalWheel );
    vLayout1->addWidget( d_timerWheel );
    vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_amplitudeKnob );
    vLayout1->addWidget( d_frequencyKnob );

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );
    layout->addLayout( vLayout1 );

    connect( d_amplitudeKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( amplitudeChanged( double ) ) );
    connect( d_frequencyKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( frequencyChanged( double ) ) );
    connect( d_timerWheel, SIGNAL( valueChanged( double ) ),
        SIGNAL( signalIntervalChanged( double ) ) );

    connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        d_plot, SLOT( setIntervalLength( double ) ) );
}

void OscWidget::start()
{
    d_plot->start();
    timer->start();
}

double OscWidget::frequency() const
{
    return d_frequencyKnob->value();
}

double OscWidget::amplitude() const
{
    return d_amplitudeKnob->value();
}

double OscWidget::signalInterval() const
{
    return d_timerWheel->value();
}
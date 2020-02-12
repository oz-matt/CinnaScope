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
#include <QApplication>
#include <QGroupBox>


extern CinnaState cstate;

OscWidget::OscWidget( QWidget *parent ):
    QWidget( parent )
{
    //const double intervalLength = 10.0; // seconds

    d_plot = new Plot( this );
    d_plot->setIntervalLength( 0.5 );

    this->setMouseTracking(true);

    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, d_plot, &Plot::updateMe);

    d_timeperdivKnob = new Knob( "Time/div", 0, 10, 50, true);
    d_timeperdivKnob->setValue( 5 );
 QGroupBox *groupBox = new QGroupBox(tr("Vertical"));
 QGroupBox *horiz_groupbox = new QGroupBox(tr("Horizontal"));
QHBoxLayout *horiz_layout = new QHBoxLayout(  );
    d_vperdivKnob = new Knob( "V/div", 0, 10, 50, true);
    d_vperdivKnob->setValue( 5 );
    d_vperdivKnob2 = new Knob( "V/div", 0, 10, 50, true);
    d_vperdivKnob2->setValue( 5 );

    d_ch1offsetknob = new Knob( "V/div", 0, 10, 30, false);
    d_ch1offsetknob->setValue( 5 );

    d_ch2offsetknob = new Knob( "V/div", 0, 10, 30, false);
    d_ch2offsetknob->setValue( 5 );

    xline = new QwtPlotCurve();

    double x[3] ={1.0, 0.0, 1.0};
        double y[3] ={1.0,0,0};

        xline->setData(new QwtCPointerData(x,y,3));
        xline->setPen(QPen(QColor(Qt::white),1,Qt::SolidLine));
        xline->attach(d_plot);

        led = new QLed(  );

        led->setOffColor(QLed::ledColor::Blue);
        led->setOnColor(QLed::ledColor::Green);
        led->setShape(QLed::ledShape::Rounded);
        led->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

        led2 = new QLed(  );

        led2->setOffColor(QLed::ledColor::Blue);
        led2->setOnColor(QLed::ledColor::Green);
        led2->setShape(QLed::ledShape::Rounded);
        led2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        ledch1on = new QLed(  );

        ledch1on->setOffColor(QLed::ledColor::Blue);
        ledch1on->setOnColor(QLed::ledColor::Green);
        ledch1on->setShape(QLed::ledShape::Rounded);
        ledch1on->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        ledch1on->setValue(true);

        ledch2on = new QLed(  );

        ledch2on->setOffColor(QLed::ledColor::Blue);
        ledch2on->setOnColor(QLed::ledColor::Green);
        ledch2on->setShape(QLed::ledShape::Rounded);
        ledch2on->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

        d_ch1offsetknob->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        d_ch1offsetknob->resize(80,80);

        QLabel* vert_labelch = new QLabel("On");
        QLabel* vert_label1 = new QLabel("Scale");
        QLabel* vert_label2 = new QLabel("Trig");
        QLabel* vert_label3 = new QLabel("Offset");

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    //vLayout1->addWidget( d_intervalWheel );
    //vLayout1->addWidget( d_timerWheel );
    //vLayout1->addStretch( 10 );
    horiz_layout->addWidget( d_timeperdivKnob );
    horiz_groupbox->setLayout(horiz_layout);

    //horiz_layout->setSizeConstraint(QLayout::SetFixedSize);
    d_timeperdivKnob->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
    //vLayout1->addWidget( led, 1 );

    QVBoxLayout *vert_layout = new QVBoxLayout(  );

    QHBoxLayout *ChOnRow = new QHBoxLayout(  );
    QHBoxLayout *ScaleRow = new QHBoxLayout(  );
    QHBoxLayout *TrigRow = new QHBoxLayout(  );
    QHBoxLayout *OffsetRow = new QHBoxLayout(  );

    ChOnRow->addWidget( ledch1on);
    ChOnRow->addWidget(vert_labelch, 0,Qt::AlignCenter);
    ChOnRow->addWidget( ledch2on);

    ChOnRow->setContentsMargins(QMargins(30,0,30,0));
    ChOnRow->setSizeConstraint(QLayout::SetMinimumSize);

    ScaleRow->addWidget( d_vperdivKnob);
    ScaleRow->addWidget(vert_label1, 0,Qt::AlignCenter);
    ScaleRow->addWidget( d_vperdivKnob2);

    TrigRow->addWidget( led);
    TrigRow->addWidget(vert_label2, 0,Qt::AlignCenter);
    TrigRow->addWidget( led2);

    TrigRow->setContentsMargins(QMargins(30,0,30,0));

    OffsetRow->addWidget( d_ch1offsetknob);
    OffsetRow->addWidget(vert_label3, 0,Qt::AlignCenter);
    OffsetRow->addWidget( d_ch2offsetknob);

    OffsetRow->setContentsMargins(QMargins(17,0,17,0));

    vert_layout->addLayout( ChOnRow);
    vert_layout->addLayout( ScaleRow);
    vert_layout->addLayout( TrigRow);
    vert_layout->addLayout( OffsetRow);

    vert_layout->setSizeConstraint(QLayout::SetFixedSize);
    groupBox->setLayout(vert_layout);

    vLayout1->addWidget( horiz_groupbox );
    vLayout1->addWidget( groupBox );

    led->resize(QSize(60,20));

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

    connect( ledch1on, SIGNAL( pushed(void ) ),
            d_plot, SLOT(  toggleChannel(void) ) );
    /*connect( xline, SIGNAL( mousePressEvent(QMouseEvent*) ),
            this, SLOT( StartMouseDragListen_tl(QMouseEvent*) ) );
    connect( xline, SIGNAL( mouseMoveEvent(QMouseEvent*) ),
            this, SLOT( ContinueMouseDragListen_tl(QMouseEvent*) ) );
    connect( xline, SIGNAL( mouseReleaseEvent(QMouseEvent*) ),
            this, SLOT( StopMouseDragListen_tl(QMouseEvent*) ) );
*/
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

void OscWidget::StartMouseDragListen_tl(QMouseEvent *event)
{
    this->setCursor(Qt::SplitVCursor);
}

void OscWidget::ContinueMouseDragListen_tl(QMouseEvent *event)
{

}

void OscWidget::StopMouseDragListen_tl(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
}

extern bool unsetOnce;
extern bool setOnce;

void OscWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (unsetOnce)
    {
        qApp->restoreOverrideCursor();
        unsetOnce = false;
        setOnce = true;
    }
}



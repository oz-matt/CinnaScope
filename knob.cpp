#include "knob.h"
#include <qwt_math.h>
#include <qpen.h>
#include <qwt_knob.h>
#include <qwt_round_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qevent.h>
#include <QDebug>
#include <cinnafixedknob.h>
#include "cinnastate.h"
#include "oscwidget.h"

extern CinnaState cstate;
extern double posknob;

Knob::Knob( const QString &title, double min, double max, int knobWidth, bool drawTicks, QWidget *parent ):
    QWidget( parent )
{
    QFont font( "Helvetica", 10 );

    d_knob = new CinnaFixedKnob( this, drawTicks );
    d_knob->setFont( font );
    //d_knob->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    this->knob_width = knobWidth;
    this->draw_ticks = drawTicks;

    //QwtScaleDiv scaleDiv =
    //    d_knob->scaleEngine()->divideScale( min, max, 5, 3 );

    /*QList<double> ticks = scaleDiv.ticks( QwtScaleDiv::MajorTick );
    if ( ticks.size() > 0 && ticks[0] > min )
    {
        if ( ticks.first() > min )
            ticks.prepend( min );
        if ( ticks.last() < max )
            ticks.append( max );
    }*/
    //scaleDiv.setTicks( QwtScaleDiv::MajorTick, ticks );
    //d_knob->setScale( scaleDiv );

    d_knob->setKnobWidth( knobWidth );

    font.setBold( true );
    //d_label = new QLabel( title, this );
    //d_label->setFont( font );
    //d_label->setAlignment( Qt::AlignTop | Qt::AlignHCenter );

    d_knob->setKnobStyle(QwtKnob::Styled);
d_knob->setMarkerStyle(QwtKnob::Tick);
    //setSizePolicy( QSizePolicy::MinimumExpanding,
    //    QSizePolicy::MinimumExpanding );

    //connect( d_knob, SIGNAL( valueChanged( double ) ),
    //    this, SIGNAL( valueChanged( double ) ) );

    //connect( d_knob, SIGNAL( wheelEvent(QWheelEvent *event) ),
    //    this, SLOT( updateTimePerDivText(QWheelEvent *event) ) );

    connect( d_knob, SIGNAL( mousePressEvent(QMouseEvent*) ),
            this, SIGNAL( StartMouseDragListen(QMouseEvent*) ) );
    connect( d_knob, SIGNAL( mouseMoveEvent(QMouseEvent*) ),
            this, SIGNAL( ContinueMouseDragListen(QMouseEvent*) ) );

}

/*void Knob::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    qDebug("y: %d", numDegrees.y());

    if (numDegrees.y()  > 0)
    {
        posknob += 2;
        d_knob->setValue(posknob);
        if (posknob >= 20) posknob = 2;
        cstate.incrementTimePerDiv();
    }
    else
    {
        posknob -= 2;
        d_knob->setValue(posknob);
        if (posknob < 2) posknob = 20;
        cstate.decrementTimePerDiv();
    }

}*/

QSize Knob::sizeHint() const
{
    QSize sz1 = d_knob->sizeHint();
    //QSize sz2 = d_label->sizeHint();

    const int w = sz1.width();//qMax( sz1.width(), sz2.width() );
    const int h = sz1.height(); //+ sz2.height();

    return QSize( w, h );
}

void Knob::setValue( double value )
{
    d_knob->setValue( value );
}

double Knob::value() const
{
    return d_knob->value();
}

void Knob::setTheme( const QColor &color )
{
    d_knob->setPalette( color );
}

QColor Knob::theme() const
{
    return d_knob->palette().color( QPalette::Window );
}

void Knob::resizeEvent( QResizeEvent *event )
{
    const QSize sz = event->size();
    //const QSize hint = d_label->sizeHint();

    //d_label->setGeometry( 0, sz.height() - hint.height(),
    //    sz.width(), hint.height() );

    const int knobHeight = d_knob->sizeHint().height();

    int off = qCeil( d_knob->scaleDraw()->extent( d_knob->font() ) );
    off -= 15 + (50 - this->knob_width); // spacing

    if (!this->draw_ticks) off -= 10;

    d_knob->setGeometry( 0, sz.height() - knobHeight + off,
        sz.width(), knobHeight );
}

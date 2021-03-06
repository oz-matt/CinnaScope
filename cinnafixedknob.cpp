#include "cinnafixedknob.h"
#include "cinnaroundscaledraw.h"
#include <QDebug>
#include <qevent.h>

CinnaFixedKnob::CinnaFixedKnob( QWidget* parent, bool drawTicks ):
    QwtKnob( parent )
{

    if(drawTicks)
        this->setScaleDraw(new CinnaRoundScaleDraw());
    else
        this->setScaleDraw(new CinnaRoundScaleDrawNoTicks());


    this->setTotalAngle( 360.0 );

    this->setScale( 0.0, 10.0 );
    this->setValue( 5.0 );

}

CinnaFixedKnob::~CinnaFixedKnob()
{

}

double CinnaFixedKnob::scrolledTo( const QPoint &pos ) const
{

}

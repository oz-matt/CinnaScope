#include "cinnafixedknob.h"
#include "cinnaroundscaledraw.h"
#include <QDebug>
#include <qevent.h>

CinnaFixedKnob::CinnaFixedKnob( QWidget* parent ):
    QwtKnob( parent )
{

    this->setScaleDraw(new CinnaRoundScaleDraw());

    this->setTotalAngle( 360.0 );

    this->setScale( 0.0, 10.0 );
    this->setValue( 5.0 );

}

CinnaFixedKnob::~CinnaFixedKnob()
{

}
void CinnaFixedKnob::mouseMoveEvent(QMouseEvent * event)
{
    qDebug("y is: %d, x is: %d", event->y(), event->x());
}

double CinnaFixedKnob::scrolledTo( const QPoint &pos ) const
{

}


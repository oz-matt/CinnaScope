#include "cinnafixedknob.h"
#include "cinnaroundscaledraw.h"

CinnaFixedKnob::CinnaFixedKnob( QWidget* parent ):
    QwtKnob( parent )
{



    this->setScaleDraw(new CinnaRoundScaleDraw());
}

CinnaFixedKnob::~CinnaFixedKnob()
{

}

double CinnaFixedKnob::scrolledTo( const QPoint &pos ) const
{

}

#ifndef CINNAFIXEDKNOB_H
#define CINNAFIXEDKNOB_H

#include <QWidget>
#include <qwt_knob.h>


class CinnaFixedKnob : public QwtKnob
{
public:
    CinnaFixedKnob( QWidget* parent = NULL );
    virtual ~CinnaFixedKnob();


protected:

    double scrolledTo( const QPoint &pos ) const;

};

#endif // CINNAFIXEDKNOB_H

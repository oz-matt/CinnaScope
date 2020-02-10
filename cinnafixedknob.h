#ifndef CINNAFIXEDKNOB_H
#define CINNAFIXEDKNOB_H

#include <QWidget>
#include <qwt_knob.h>
#include <QObject>

class CinnaFixedKnob : public QwtKnob
{

    Q_OBJECT

public:
    CinnaFixedKnob(QWidget* parent = NULL , bool drawTicks = true);
    virtual ~CinnaFixedKnob();

Q_SIGNALS:
    void mouseMoveEvent( QMouseEvent * event);
    void mousePressEvent( QMouseEvent * event);
    void mouseReleaseEvent( QMouseEvent * event);

protected:
    double scrolledTo( const QPoint &pos ) const;

};

#endif // CINNAFIXEDKNOB_H

#ifndef CINNAROUNDSCALEDRAW_H
#define CINNAROUNDSCALEDRAW_H

#include <QWidget>
#include <qwt_round_scale_draw.h>

class CinnaRoundScaleDraw : public QwtRoundScaleDraw
{
public:
    CinnaRoundScaleDraw();

protected:
    virtual void drawLabel( QPainter *, double val ) const;
};

#endif // CINNAROUNDSCALEDRAW_H

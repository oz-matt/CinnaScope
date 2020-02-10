#ifndef CINNAROUNDSCALEDRAW_H
#define CINNAROUNDSCALEDRAW_H

#include <QWidget>
#include <qwt_round_scale_draw.h>

class CinnaRoundScaleDraw : public QwtRoundScaleDraw
{
public:
    CinnaRoundScaleDraw();

    void drawLabel( QPainter *, double val ) const;
    //void drawTick( QPainter *, double value, double len ) const;
    //void drawBackbone( QPainter * ) const;
};

class CinnaRoundScaleDrawNoTicks : public QwtRoundScaleDraw
{
public:
    CinnaRoundScaleDrawNoTicks(){}

    void drawLabel( QPainter *, double val ) const{}
    void drawTick( QPainter *, double value, double len ) const{}
    //void drawBackbone( QPainter * ) const;
};

#endif // CINNAROUNDSCALEDRAW_H

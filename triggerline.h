#ifndef TRIGGERLINE_H
#define TRIGGERLINE_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <qwt_plot_curve.h>

class TriggerLine : public QWidget, public QwtPlotCurve
{

    Q_OBJECT

public:
    TriggerLine();

    virtual ~TriggerLine();

Q_SIGNALS:
    void mouseMoveEvent( QMouseEvent * event);
    void mousePressEvent( QMouseEvent * event);
    void mouseReleaseEvent( QMouseEvent * event);

};

#endif // TRIGGERLINE_H

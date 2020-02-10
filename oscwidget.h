#include <qwidget.h>
#include <qtimer.h>
#include <qlabel.h>
#include "triggerline.h"
#include <qwt_plot_curve.h>
#include "qled.h"


class Plot;
class Knob;
class WheelBox;

class OscWidget : public QWidget
{
    Q_OBJECT

public:
    OscWidget( QWidget * = NULL );

    void start();

    double amplitude() const;
    double frequency() const;
    double signalInterval() const;

    void incrementTimePerDiv();
    void decrementTimePerDiv();
    void incrementVPerDiv();
    void decrementVPerDiv();

    void mouseMoveEvent(QMouseEvent* event);

Q_SIGNALS:
    void amplitudeChanged( double );
    void frequencyChanged( double );
    void signalIntervalChanged( double );

public Q_SLOTS:
    void updateTimePerDivTextFromScroll(QWheelEvent *event);
    void StartMouseDragListen(QMouseEvent *event);
    void ContinueMouseDragListen(QMouseEvent *event);

    void updateTimePerDivTextFromScroll_vpd(QWheelEvent *event);
    void StartMouseDragListen_vpd(QMouseEvent *event);
    void ContinueMouseDragListen_vpd(QMouseEvent *event);

    void StartMouseDragListen_tl(QMouseEvent *event);
    void ContinueMouseDragListen_tl(QMouseEvent *event);
    void StopMouseDragListen_tl(QMouseEvent *event);

private:
    Knob *d_vperdivKnob;
    Knob *d_vperdivKnob2;
    Knob *d_ch1offsetknob;
    Knob *d_timeperdivKnob;
QLed* led;

    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;

    Plot *d_plot;
    QTimer *timer;

    QLabel* tpd_label;
    QLabel* vpd_label;

    QwtPlotCurve *xline;
};

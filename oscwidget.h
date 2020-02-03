#include <qwidget.h>
#include <qtimer.h>
#include <qlabel.h>

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


Q_SIGNALS:
    void amplitudeChanged( double );
    void frequencyChanged( double );
    void signalIntervalChanged( double );

public Q_SLOTS:
    void updateTimePerDivTextFromScroll(QWheelEvent *event);

    void StartMouseDragListen(QMouseEvent *event);
    void ContinueMouseDragListen(QMouseEvent *event);
    void StopMouseDragListen(QMouseEvent *event);
private:
    Knob *d_frequencyKnob;
    Knob *d_amplitudeKnob;
    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;

    Plot *d_plot;
    QTimer *timer;
    QLabel* le;
};

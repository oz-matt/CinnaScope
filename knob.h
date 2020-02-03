#ifndef _KNOB_H_
#define _KNOB_H_

#include <qwidget.h>

class CinnaFixedKnob;
class QLabel;

class Knob: public QWidget
{
    Q_OBJECT

    Q_PROPERTY( QColor theme READ theme WRITE setTheme )

public:
    Knob( const QString &title,
        double min, double max, QWidget *parent = NULL );


    virtual QSize sizeHint() const;

    void setValue( double value );
    double value() const;

    void setTheme( const QColor & );
    QColor theme() const;

Q_SIGNALS:
    //double valueChanged( double );
    void wheelEvent(QWheelEvent *event);
    //void mouseMoveEvent( QMouseEvent * event);
    //void mousePressEvent( QMouseEvent * event);

//public Q_SLOTS:
    void StartMouseDragListen(QMouseEvent *event);
    void ContinueMouseDragListen(QMouseEvent *event);
    void StopMouseDragListen(QMouseEvent *event);

protected:
    virtual void resizeEvent( QResizeEvent * );

private:
    CinnaFixedKnob *d_knob;
    QLabel *d_label;
};

#endif

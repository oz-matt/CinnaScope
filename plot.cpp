#include "plot.h"
#include "curvedata.h"
#include "signaldata.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>
#include <qevent.h>
#include<QDebug>
#include <QApplication>

class Canvas: public QwtPlotCanvas
{
public:
    Canvas( QwtPlot *plot = NULL ):
        QwtPlotCanvas( plot )
    {
        // The backing store is important, when working with widget
        // overlays ( f.e rubberbands for zooming ).
        // Here we don't have them and the internal
        // backing store of QWidget is good enough.

        setPaintAttribute( QwtPlotCanvas::BackingStore, false );
        setBorderRadius( 10 );

        if ( QwtPainter::isX11GraphicsSystem() )
        {
#if QT_VERSION < 0x050000
            // Even if not liked by the Qt development, Qt::WA_PaintOutsidePaintEvent
            // works on X11. This has a nice effect on the performance.

            setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif

            // Disabling the backing store of Qt improves the performance
            // for the direct painter even more, but the canvas becomes
            // a native window of the window system, receiving paint events
            // for resize and expose operations. Those might be expensive
            // when there are many points and the backing store of
            // the canvas is disabled. So in this application
            // we better don't disable both backing stores.

            if ( testPaintAttribute( QwtPlotCanvas::BackingStore ) )
            {
                setAttribute( Qt::WA_PaintOnScreen, true );
                setAttribute( Qt::WA_NoSystemBackground, true );
            }
        }

        setupPalette();
    }

private:
    void setupPalette()
    {
        QPalette pal = palette();

#if QT_VERSION >= 0x040400
        QLinearGradient gradient;
        gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
        gradient.setColorAt( 0.0, QColor( 0, 49, 110 ) );
        gradient.setColorAt( 1.0, QColor( 0, 87, 174 ) );

        pal.setBrush( QPalette::Window, QBrush( gradient ) );
#else
        pal.setBrush( QPalette::Window, QBrush( color ) );
#endif

        // QPalette::WindowText is used for the curve color
        pal.setColor( QPalette::WindowText, Qt::green );

        setPalette( pal );
    }
};

Plot::Plot( QWidget *parent ):
    QwtPlot( parent ),
    d_paintedPoints( 0 ),
    d_interval( 0.0, 0.01 ),
    d_timerId( -1 )
{
    d_directPainter = new QwtPlotDirectPainter();

    setAutoReplot( false );

    Canvas* canv = new Canvas();
    canv->setMouseTracking(true);
    setCanvas(canv);

    plotLayout()->setAlignCanvasToScales( true );

    //setAxisTitle( QwtPlot::xBottom, "Time [s]" );
    setAxisScale( QwtPlot::xBottom, d_interval.minValue(), d_interval.maxValue() );
    setAxisScale( QwtPlot::yLeft, -2.0, 2.0 );

    d_origin = new QwtPlotMarker();
    //d_origin->setLineStyle( QwtPlotMarker::Cross );
    d_origin->setValue( d_interval.minValue() + d_interval.width() / 2.0, 0.0 );
    //d_origin->setLinePen( Qt::gray, 0.0, Qt::DashLine );
    d_origin->attach( this );

    d_curve = new QwtPlotCurve();
    d_curve->setStyle( QwtPlotCurve::Lines );
    d_curve->setPen( canvas()->palette().color( QPalette::WindowText ) );
    d_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    d_curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    d_curve->setData( new CurveData() );
    d_curve->attach( this );

    QwtPlot::enableAxis(QwtPlot::xBottom, false);
    QwtPlot::enableAxis(QwtPlot::yLeft, false);
    //QwtPlot::setAxisMaxMinor(QwtPlot::xBottom, 16);
    QwtPlot::setAxisMaxMajor(QwtPlot::xBottom, 16);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( false );
    grid->enableY( true );
    grid->enableYMin( false );
    grid->attach( this );
this->setMouseTracking(true);
}

Plot::~Plot()
{
    delete d_directPainter;
}

void Plot::start()
{
    d_clock.start();
    d_timerId = startTimer( 10 );
}

void Plot::replot()
{
    CurveData *curveData = static_cast<CurveData *>( d_curve->data() );
    curveData->values().lock();

    QwtPlot::replot();
    d_paintedPoints = curveData->size();

    curveData->values().unlock();
}

void Plot::setIntervalLength( double interval )
{
    if ( interval > 0.0 && interval != d_interval.width() )
    {
        d_interval.setMaxValue( d_interval.minValue() + interval );
        //setAxisScale( QwtPlot::xBottom,
        //    d_interval.minValue(), d_interval.maxValue() );

        QwtScaleDiv div(d_interval.minValue(), d_interval.maxValue());

        double inc = ((d_interval.maxValue() - d_interval.minValue()) / 10.0);

                //QList<double> majorTicks;
                //majorTicks << 0 << 75 << 150;

                QList<double> majorTicks;
                //mediumTicks << 15 << 30 << 45 << 60 << 90 << 105 << 120 << 135;

                int i;
                for(i=0;i<10;i++)
                {
                    majorTicks << d_interval.minValue() + (i * inc);
                }

                //div.setTicks(QwtScaleDiv::MajorTick, majorTicks);
                div.setTicks(QwtScaleDiv::MajorTick, majorTicks);

                setAxisScaleDiv(QwtPlot::xBottom, div);

                replot();
    }
}

void Plot::updateMe()
{
    updateCurve();
}

void Plot::updateCurve()
{
    CurveData *curveData = static_cast<CurveData *>( d_curve->data() );
    curveData->values().lock();

    const int numPoints = curveData->size();
    if ( numPoints > d_paintedPoints )
    {
        const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
        if ( doClip )
        {
            /*
                Depending on the platform setting a clip might be an important
                performance issue. F.e. for Qt Embedded this reduces the
                part of the backing store that has to be copied out - maybe
                to an unaccelerated frame buffer device.
            */

            const QwtScaleMap xMap = canvasMap( d_curve->xAxis() );
            const QwtScaleMap yMap = canvasMap( d_curve->yAxis() );

            QRectF br = qwtBoundingRect( *curveData,
                d_paintedPoints - 1, numPoints - 1 );

            const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
            d_directPainter->setClipRegion( clipRect );
        }

        d_directPainter->drawSeries( d_curve,
            d_paintedPoints - 1, numPoints - 1 );
        d_paintedPoints = numPoints;
    }

    curveData->values().unlock();
}

void Plot::incrementInterval()
{
    d_interval = QwtInterval( d_interval.maxValue(),
        d_interval.maxValue() + d_interval.width() );

    CurveData *curveData = static_cast<CurveData *>( d_curve->data() );
    curveData->values().clearStaleValues( d_interval.minValue() );

    // To avoid, that the grid is jumping, we disable
    // the autocalculation of the ticks and shift them
    // manually instead.

    QwtScaleDiv scaleDiv = axisScaleDiv( QwtPlot::xBottom );
    scaleDiv.setInterval( d_interval );

    for ( int i = 0; i < QwtScaleDiv::NTickTypes; i++ )
    {
        QList<double> ticks = scaleDiv.ticks( i );
        for ( int j = 0; j < ticks.size(); j++ )
            ticks[j] += d_interval.width();
        scaleDiv.setTicks( i, ticks );
    }


    setAxisScaleDiv( QwtPlot::xBottom, scaleDiv );

    d_origin->setValue( d_interval.minValue() + d_interval.width() / 2.0, 0.0 );

    d_paintedPoints = 0;
    replot();
}

extern double curr_time;

void print_bram_address_ptr(void)
{
    //if (PCIE_DmaRead(hPCIe, LocalAddr, pRead, nTestSize)
}

void Plot::timerEvent( QTimerEvent *event )
{
    if ( event->timerId() == d_timerId )
    {
        updateCurve();

        if ( curr_time > (d_interval.maxValue() + (d_interval.maxValue() - d_interval.minValue())) )
        {
            incrementInterval();
            qDebug("Maxval: %f      Minval: %f", d_interval.maxValue(), d_interval.minValue());
        }
        return;
        /*double maxval = d_interval.maxValue();
        double minval = d_interval.minValue();

        if ( elapsed > (maxval + (maxval - minval)) )
            qDebug("Maxval: %f      Minval: %f", maxval, minval);
            incrementInterval();
*/

    }

    QwtPlot::timerEvent( event );
}

void Plot::resizeEvent( QResizeEvent *event )
{
    d_directPainter->reset();
    QwtPlot::resizeEvent( event );
}

void Plot::showEvent( QShowEvent * )
{
    replot();
}

bool Plot::eventFilter( QObject *object, QEvent *event )
{
    if ( object == canvas() &&
        event->type() == QEvent::PaletteChange )
    {
        d_curve->setPen( canvas()->palette().color( QPalette::WindowText ) );
    }

    return QwtPlot::eventFilter( object, event );
}

bool unsetOnce = false;
bool setOnce = true;
bool triglineclicked = false;


void Plot::mouseMoveEvent( QMouseEvent * event)
{

    double centerHeight = this->frameRect().height() / 2.0;

    double lowerBound = centerHeight - 3;
    double upperBound = centerHeight + 3;

    if(triglineclicked)
    {
        trigLineDrag(event, this->frameRect().height());
    }
    else
    {
        if((event->y() <= upperBound) && (event->y() >= lowerBound))
        {
            if (setOnce)
            {
                qDebug("y: %d, ysize: %d", event->y(), this->frameRect().height());
                qApp->setOverrideCursor(Qt::SplitVCursor);
                unsetOnce = true;
                setOnce = false;
            }
        }
        else
        {
            if (unsetOnce)
            {
                qApp->restoreOverrideCursor();
                unsetOnce = false;
                setOnce = true;
            }
        }
    }

}

void Plot::mousePressEvent( QMouseEvent * event)
{
    if (unsetOnce)
    {
        qDebug("clicked!");
        triglineclicked = true;
    }
}

void Plot::mouseReleaseEvent( QMouseEvent * event)
{
    double centerHeight = this->frameRect().height() / 2.0;

    double lowerBound = centerHeight - 3;
    double upperBound = centerHeight + 3;

    triglineclicked = false;
    if(!((event->y() <= upperBound) && (event->y() >= lowerBound)))
    {
        qApp->restoreOverrideCursor();
        unsetOnce = false;
        setOnce = true;
    }
}


bool chon = true;

void Plot::toggleChannel()
{
    if(chon)
    {
        d_curve->detach(  );
        replot();
        chon = false;
    }
    else
    {
        d_curve->attach( this );
        replot();
        chon = true;
    }
}


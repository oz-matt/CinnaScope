#include "samplingthread.h"
#include "signaldata.h"
#include "plot.h"
#include "dummydata.h"
#include <qwt_math.h>
#include <math.h>

INCLUDE_DUMMY_DATA;

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread::SamplingThread( QObject *parent ):
    QwtSamplingThread( parent ),
    d_frequency( 5.0 ),
    d_amplitude( 20.0 )
{
}

void SamplingThread::setFrequency( double frequency )
{
    d_frequency = frequency;
}

double SamplingThread::frequency() const
{
    return d_frequency;
}

void SamplingThread::setAmplitude( double amplitude )
{
    d_amplitude = amplitude;
}

double SamplingThread::amplitude() const
{
    return d_amplitude;
}

void SamplingThread::sample( double elapsed )
{
    if ( d_frequency > 0.0 )
    {
        const QPointF s( elapsed, value( elapsed ) );
        SignalData::instance().append( s );
    }
}

int i = 0;

double SamplingThread::value( double timeStamp ) const
{
    //const double period = 1.0 / d_frequency;

    //const double x = ::fmod( timeStamp, period );
    //const double v = d_amplitude * qFastSin( x / period * 2 * M_PI );

    const double v = dummydata[i++];
    if (i > 999) i = 0;

    return v;
}

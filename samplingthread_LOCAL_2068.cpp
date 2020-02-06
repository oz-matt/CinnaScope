#include "samplingthread.h"
#include "signaldata.h"
#include "plot.h"
#include "dummydata.h"
#include <qwt_math.h>
#include <math.h>
#include <QDebug>

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

//Right now we set sample() to be called every 10ms (100 times/sec)
#define SAMPLE_FXN_CALLS_PER_SEC 100
#define INCOMING_SAMPLES_PER_SEC 200
#define DATA_PTS_COLLECTED_PER_SAMPLE_FXN_CALL 2

#define TIMESTEP .005


double curr_time = 0;

void SamplingThread::sample( double elapsed )
{
    if ( d_frequency > 0.0 )
    {
        int i;
        for(i=0;i<DATA_PTS_COLLECTED_PER_SAMPLE_FXN_CALL;i++){

            double val = value(0);
            const QPointF s( curr_time, val );
            SignalData::instance().append( s );
            curr_time = curr_time + TIMESTEP;


            //qDebug("curr_time: %f   val: %f", curr_time, val);
        }
    }
}

int i = 0;

double SamplingThread::value( double timeStamp ) const
{
    //const double period = 1.0 / d_frequency;

    //const double x = ::fmod( timeStamp, period );
    //const double v = d_amplitude * qFastSin( x / period * 2 * M_PI );

    const double v = dummydata[i++];//(double)5.0 * dummydata[i++];
    if (i > 999) i = 0;

    return v;
}

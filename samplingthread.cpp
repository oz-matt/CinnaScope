#include "samplingthread.h"
#include "signaldata.h"
#include "plot.h"
#include "dummydata.h"
#include <qwt_math.h>
#include <math.h>
#include <QDebug>
#include "cinnapcieinterface.h"
#include <QQueue>

extern CinnaPcieInterface cpi;

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
#define INCOMING_SAMPLES_PER_SEC 100
#define DATA_PTS_COLLECTED_PER_SAMPLE_FXN_CALL 2

#define TIMESTEP 0.000000064


double curr_time = 0;
bool firstSample = true;

//extern QQueue<WORD> dataq;

extern WORD dataq[100];
extern WORD dataq_address;
extern WORD dataq_num_new_pts;

void SamplingThread::sample( double elapsed )
{

    while(1)
    {

        cpi.lockDataMutex();
        if(dataq_num_new_pts > 0)
        {
            /*qint16 ytc = (qint16)(dataq.dequeue() << 2) / 4; // convert to 2s comp
            cpi.unlockDataMutex();

            double yvolts = ytc * (double)0.01220703125;

            const QPointF s(curr_time, yvolts);
            SignalData::instance().append(s);
            curr_time += TIMESTEP;*/
        }
        else
        {
            cpi.unlockDataMutex();
            break;
        }

    }
    //if(dataq.isEmpty())qDebug("lempty");
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

#include "samplingthread.h"
#include "signaldata.h"
#include "plot.h"
#include "dummydata.h"
#include <qwt_math.h>
#include <math.h>
#include <QDebug>
#include "cinnapcieinterface.h"

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

#define TIMESTEP .01


double curr_time = 0;

DWORD lastAddress = 0;

void SamplingThread::sample( double elapsed )
{
    if ( d_frequency > 0.0 )
    {
        int i;
        for(i=0;i<DATA_PTS_COLLECTED_PER_SAMPLE_FXN_CALL;i++){

            DWORD address = 0;

            if(cpi.Get_BRAM_Address_Pointer(&address))
            {
                if(cpi.updateOscData())
                {
                    bool wrap = false;
                    int wrap_spacing = 0;

                    //qDebug("Address: %i, Data: %d", address, cpi.pcie_read_data[address]);
                    int numNewPoints = 0;

                    if((signed int)(address - lastAddress) < 0)
                    {
                        wrap = true;
                        wrap_spacing = 16384 - lastAddress;
                        numNewPoints = wrap_spacing + address + 1;
                    }
                    else
                    {
                        numNewPoints = address - lastAddress;
                    }

                    //qDebug("Num Points To Update: %d, wrap: %b", numNewPoints, wrap);
                    lastAddress = address;

                    if (wrap)
                    {
                        int j, k;
                        for(j=(wrap_spacing-1);j>=0;j--)
                        {
                            const QPointF s( curr_time, cpi.pcie_read_data[16384 - j]);
                            SignalData::instance().append( s );
                            curr_time = curr_time + TIMESTEP;
                        }
                        for(k=address;k>=0;k--)
                        {
                            const QPointF s( curr_time, cpi.pcie_read_data[address - k]);
                            SignalData::instance().append( s );
                            curr_time = curr_time + TIMESTEP;
                        }
                    }
                    else
                    {
                        qDebug("\r\nSample START:\r\n\r\n");
                        qDebug("Num Points To Update: %d", numNewPoints);

                        int j;
                        for(j=(numNewPoints-1);j>=0;j--)
                        {
                            DWORD y = (cpi.pcie_read_data[address - j]) >> 48;
                            double yvolts = (y - 8192) * 0.01220703125;

                            const QPointF s( curr_time, yvolts);
                            SignalData::instance().append( s );
                            curr_time += TIMESTEP;
                            qDebug("Address: %d, Data: %hd, Volts: %f", address, y, yvolts);
                        }

                        qDebug("\r\nEarlier:");
                        qDebug("Address: %d, Data: %llX", address - j - 2, cpi.pcie_read_data[address - j - 2]);
                        qDebug("Address: %d, Data: %llX", address - j - 3, cpi.pcie_read_data[address - j - 3]);
                        qDebug("Address: %d, Data: %llX", address - j - 4, cpi.pcie_read_data[address - j - 4]);
                    }
                }
                else
                {
                    qDebug("FAILED TO GET DATA!!!");
                }

            }
            else
            {
                qDebug("FAILED TO GET ADDRESS!!!");
            }
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

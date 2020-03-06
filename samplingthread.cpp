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

#define TIMESTEP 0.000000064


double curr_time = 0;
bool firstSample = true;

DWORD lastAddress = 0;

static void ConvertAndAppendData(quint64 data, int address)
{
    WORD y = (data) >> 48;
    qint16 ytc = (qint16)(y << 2) / 4 ; // convert 2s comp to signed int
    double yvolts = ytc * (double)0.01220703125;

    const QPointF s( curr_time, yvolts);
    SignalData::instance().append( s );
    curr_time += TIMESTEP;
    qDebug("Address: %u, Data: %X, Volts: %f", address, data, yvolts);
}

void SamplingThread::sample( double elapsed )
{

    DWORD address = 0;

    if(cpi.Get_BRAM_Address_Pointer(&address))
    {

                bool wrap = false;
                int wrap_spacing = 0;

                qDebug("Address: %i", address);
                int numNewPoints = 0;

                if((signed int)(address - lastAddress) < 0)
                {
                    wrap = true;
                    wrap_spacing = 32767 - lastAddress;
                    numNewPoints = wrap_spacing + address + 1;
                }
                else
                {
                    numNewPoints = address - lastAddress;
                }


                lastAddress = address;
                if(firstSample) firstSample = false;
                else
                {
                //qDebug("Num Points To Update: %d, wrap: %d", numNewPoints, wrap);

                if (wrap)
                {
                    if (wrap_spacing > 0)
                    {
                        quint64* newpts1 = new quint64[wrap_spacing];

                        unsigned int start_address = 0x100000 + lastAddress + 1;

                        if(PCIE_DmaRead(cpi.getHandle(), start_address, newpts1, wrap_spacing))
                        {
                            int j;
                            for(j=0;j<wrap_spacing;j++)
                            {
                                ConvertAndAppendData(newpts1[j], lastAddress+j+1);
                            }
                        }
                        else
                        {
                            qDebug("FAILtwra1p");
                        }
                        delete newpts1;
                    }

                    quint64* newpts2 = new quint64[address+1];

                    unsigned int start_address = 0x100000;

                    if(PCIE_DmaRead(cpi.getHandle(), 0, newpts2, address+1))
                    {
                        int j;
                        for(j=0;j<address+1;j++)
                        {
                            ConvertAndAppendData(newpts2[j], j);
                        }
                    }
                    else
                    {
                        qDebug("FAILtwra2p");
                    }
                    delete newpts2;

                }
                else
                {
                    qDebug("Num Points To Update: %d", numNewPoints);

                    quint64* newpts = new quint64[numNewPoints];

                    unsigned int start_address = 0x100000 + lastAddress + 1;

                    if(PCIE_DmaRead(cpi.getHandle(), start_address, newpts, numNewPoints))
                    {
                       int j;
                       for(j=0;j<numNewPoints;j++)
                       {
                           ConvertAndAppendData(newpts[j], lastAddress+1+j);
                       }
                    }
                    else
                    {
                        qDebug("FAILt");
                    }
                    delete newpts;

                }
                }


    }
    else
    {
        qDebug("FAILED TO GET ADDRESS!!!");
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

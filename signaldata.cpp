#include "signaldata.h"
#include <qvector.h>
#include <qmutex.h>
#include <qreadwritelock.h>
#include "cinnapcieinterface.h"
#include <QQueue>

extern CinnaPcieInterface cpi;

class SignalData::PrivateData
{
public:
    PrivateData():
        boundingRect( 1.0, 1.0, -2.0, -2.0 ) // invalid
    {
        values.reserve( 1000 );
    }

    inline void append( const QPointF &sample )
    {
        values.append( sample );

        // adjust the bounding rectangle

        if ( boundingRect.width() < 0 || boundingRect.height() < 0 )
        {
            boundingRect.setRect( sample.x(), sample.y(), 0.0, 0.0 );
        }
        else
        {
            boundingRect.setRight( sample.x() );

            if ( sample.y() > boundingRect.bottom() )
                boundingRect.setBottom( sample.y() );

            if ( sample.y() < boundingRect.top() )
                boundingRect.setTop( sample.y() );

        }
    }

    QReadWriteLock lock;

    QVector<QPointF> values;
    QRectF boundingRect;

    QMutex mutex; // protecting pendingValues
    QVector<QPointF> pendingValues;
};

SignalData::SignalData()
{
    d_data = new PrivateData();
}

SignalData::~SignalData()
{
    delete d_data;
}

extern QVector<WORD> dataq;

int SignalData::size() const
{
    cpi.lockDataMutex();
    int s = dataq.size();
    cpi.unlockDataMutex();

    return s;
}

double curr_time2 = 0.0;

QPointF SignalData::value( int index ) const
{
    cpi.lockDataMutex();
    WORD next = dataq[index];
    cpi.unlockDataMutex();
    qint16 ytc = (qint16)(next << 2) / 4; // convert to 2s comp
    double yvolts = ytc * (double)0.01220703125;

    const QPointF s(curr_time2, yvolts);
    curr_time2 = curr_time2 + .00001;

    return s;
}

QRectF SignalData::boundingRect() const
{
    //return
}

void SignalData::lock()
{
    d_data->lock.lockForRead();
}

void SignalData::unlock()
{
    d_data->lock.unlock();
}

void SignalData::append( const QPointF &sample )
{
    d_data->mutex.lock();
    d_data->pendingValues += sample;

    const bool isLocked = d_data->lock.tryLockForWrite();
    if ( isLocked )
    {
        const int numValues = d_data->pendingValues.size();
        const QPointF *pendingValues = d_data->pendingValues.data();

        for ( int i = 0; i < numValues; i++ )
            d_data->append( pendingValues[i] );

        d_data->pendingValues.clear();

        d_data->lock.unlock();
    }

    d_data->mutex.unlock();
}

void SignalData::clearStaleValues( double limit )
{
    d_data->lock.lockForWrite();

    d_data->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid

    const QVector<QPointF> values = d_data->values;
    d_data->values.clear();
    d_data->values.reserve( values.size() );

    int index;
    for ( index = values.size() - 1; index >= 0; index-- )
    {
        if ( values[index].x() < limit )
            break;
    }

    if ( index > 0 )
        d_data->append( values[index++] );

    while ( index < values.size() - 1 )
        d_data->append( values[index++] );

    d_data->lock.unlock();
}

SignalData &SignalData::instance()
{
    static SignalData valueVector;
    return valueVector;
}

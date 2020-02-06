#ifndef CINNASTATE_H
#define CINNASTATE_H

#include <QWidget>
#include <QString>

#define TIME_PER_DIV_MAX MS20_PER_DIV
#define TIME_PER_DIV_MIN MS100_PER_DIV

#define V_PER_DIV_MAX V5_PER_DIV
#define V_PER_DIV_MIN MV1_PER_DIV

typedef enum {
    MS100_PER_DIV,
    MS50_PER_DIV,
    MS20_PER_DIV
} time_per_div_e;

typedef enum {
    V5_PER_DIV,
    V2_PER_DIV,
    V1_PER_DIV,
    MV500_PER_DIV,
    MV200_PER_DIV,
    MV100_PER_DIV,
    MV50_PER_DIV,
    MV20_PER_DIV,
    MV10_PER_DIV,
    MV5_PER_DIV,
    MV2_PER_DIV,
    MV1_PER_DIV
} v_per_div_e;

class CinnaState
{
public:
    CinnaState();

    time_per_div_e& getTimePerDiv();
    v_per_div_e& getVPerDiv();

    void incrementTimePerDiv();
    void decrementTimePerDiv();

    void incrementVPerDiv();
    void decrementVPerDiv();

    QString getTimePerDivString();
    double getTimePerDivInterval();

    QString getVPerDivString();
    double getVPerDivNum();


protected:

    double scrolledTo( const QPoint &pos ) const;

private:

    time_per_div_e TimePerDiv;
    v_per_div_e VPerDiv;
};

#endif // CINNASTATE_H

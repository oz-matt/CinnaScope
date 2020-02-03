#ifndef CINNASTATE_H
#define CINNASTATE_H

#include <QWidget>
#include <QString>

#define TIME_PER_DIV_MAX MS20_PER_DIV
#define TIME_PER_DIV_MIN MS100_PER_DIV

typedef enum {
    MS100_PER_DIV,
    MS50_PER_DIV,
    MS20_PER_DIV
} time_per_div_e;

class CinnaState
{
public:
    CinnaState();

    time_per_div_e& getTimePerDiv();

    void incrementTimePerDiv();
    void decrementTimePerDiv();

    QString getTimePerDivString();
    double getTimePerDivInterval();

protected:

    double scrolledTo( const QPoint &pos ) const;

private:

    time_per_div_e TimePerDiv;
};

#endif // CINNASTATE_H

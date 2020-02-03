#include "cinnastate.h"

CinnaState::CinnaState()
{
    this->TimePerDiv = MS50_PER_DIV;
}

time_per_div_e& CinnaState::getTimePerDiv()
{
    return this->TimePerDiv;
}


QString CinnaState::getTimePerDivString()
{
    switch (this->TimePerDiv)
    {
    case MS100_PER_DIV:
        return "100ms/div";
        break;
    case MS50_PER_DIV:
        return "50ms/div";
        break;
    case MS20_PER_DIV:
        return "20ms/div";
        break;
    default:
        return "Failed";
        break;
    }
}

double CinnaState::getTimePerDivInterval()
{
    switch (this->TimePerDiv)
    {
    case MS100_PER_DIV:
        return 1.0;
        break;
    case MS50_PER_DIV:
        return 0.5;
        break;
    case MS20_PER_DIV:
        return 0.2;
        break;
    default:
        return 1.0;
        break;
    }
}

void CinnaState::incrementTimePerDiv()
{
    if (this->TimePerDiv < TIME_PER_DIV_MAX)
    {
        this->TimePerDiv = static_cast<time_per_div_e>(static_cast<int>(this->TimePerDiv) + 1);
    }
}

void CinnaState::decrementTimePerDiv()
{
    if (this->TimePerDiv > TIME_PER_DIV_MIN)
    {
        this->TimePerDiv = static_cast<time_per_div_e>(static_cast<int>(this->TimePerDiv) - 1);

    }
}

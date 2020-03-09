#include "cinnastate.h"

CinnaState::CinnaState()
{
    this->TimePerDiv = MS50_PER_DIV;
    this->VPerDiv = MV500_PER_DIV;
}

time_per_div_e& CinnaState::getTimePerDiv()
{
    return this->TimePerDiv;
}

v_per_div_e& CinnaState::getVPerDiv()
{
    return this->VPerDiv;
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

QString CinnaState::getVPerDivString()
{
    switch (this->VPerDiv)
    {
    case V5_PER_DIV:
        return "5V/div";
        break;
    case V2_PER_DIV:
        return "2V/div";
        break;
    case V1_PER_DIV:
        return "1V/div";
        break;
    case MV500_PER_DIV:
        return "500mv/div";
        break;
    case MV200_PER_DIV:
        return "200mv/div";
        break;
    case MV100_PER_DIV:
        return "100mv/div";
        break;
    case MV50_PER_DIV:
        return "50mv/div";
        break;
    case MV20_PER_DIV:
        return "20mv/div";
        break;
    case MV10_PER_DIV:
        return "10mv/div";
        break;
    case MV5_PER_DIV:
        return "5mv/div";
        break;
    case MV2_PER_DIV:
        return "2mv/div";
        break;
    case MV1_PER_DIV:
        return "1mv/div";
        break;
    default:
        return "Failed";
        break;
    }
}

double CinnaState::getVPerDivNum()
{
    switch (this->VPerDiv)
    {
    case V5_PER_DIV:
        return 5.0;
        break;
    case V2_PER_DIV:
        return 2.0;
        break;
    case V1_PER_DIV:
        return 1.0;
        break;
    case MV500_PER_DIV:
        return 0.5;
        break;
    case MV200_PER_DIV:
        return 0.2;
        break;
    case MV100_PER_DIV:
        return 0.1;
        break;
    case MV50_PER_DIV:
        return 0.05;
        break;
    case MV20_PER_DIV:
        return 0.02;
        break;
    case MV10_PER_DIV:
        return 0.01;
        break;
    case MV5_PER_DIV:
        return 0.005;
        break;
    case MV2_PER_DIV:
        return 0.002;
        break;
    case MV1_PER_DIV:
        return 0.001;
        break;
    default:
        return 0.1;
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

void CinnaState::incrementVPerDiv()
{
    if (this->VPerDiv < V_PER_DIV_MIN)
    {
        this->VPerDiv = static_cast<v_per_div_e>(static_cast<int>(this->VPerDiv) + 1);
    }
}

void CinnaState::decrementVPerDiv()
{
    if (this->VPerDiv > V_PER_DIV_MAX)
    {
        this->VPerDiv = static_cast<v_per_div_e>(static_cast<int>(this->VPerDiv) - 1);

    }
}

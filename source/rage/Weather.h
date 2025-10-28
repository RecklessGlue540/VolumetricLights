#pragma once

#include "../Utility.h"

namespace CWeather
{
    enum eWeatherType : uint32_t
    {
        EXTRASUNNY,
        SUNNY,
        SUNNY_WINDY,
        CLOUDY,
        RAIN,
        DRIZZLE,
        FOGGY,
        LIGHTNING,
    };

    eWeatherType* OldWeatherType = nullptr;
    eWeatherType* NewWeatherType = nullptr;
    float* InterpolationValue    = nullptr;

    eWeatherType GetOldWeatherType()
    {
        return *OldWeatherType;
    }

    eWeatherType GetNewWeatherType()
    {
        return *NewWeatherType;
    }

    float GetWeatherInterpolationValue()
    {
        return *InterpolationValue;
    }
}
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

    bool Init()
    {
        // FusionFix code, slightly modified to add version detection
        auto pattern = find_pattern("A1 ? ? ? ? 83 C4 08 8B CF", "A1 ? ? ? ? 80 3F 04");
        if (!pattern.empty())
        {
            CWeather::OldWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
        }
        else
        {
            return false;
        }

        pattern = find_pattern("A1 ? ? ? ? 89 46 4C A1", "A1 ? ? ? ? 77 05 A1 ? ? ? ? 80 3F 04");
        if (!pattern.empty())
        {
            CWeather::NewWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
        }
        else
        {
            return false;
        }

        pattern = hook::pattern("F3 0F 10 05 ? ? ? ? 8B 44 24 0C 8B 4C 24 04");
        if (!pattern.empty())
        {
            CWeather::InterpolationValue = *pattern.get_first<float*>(4);
        }
        else
        {
            return false;
        }

        return true;
    }

    static eWeatherType GetOldWeatherType()
    {
        return *OldWeatherType;
    }

    static eWeatherType GetNewWeatherType()
    {
        return *NewWeatherType;
    }

    static float GetWeatherInterpolationValue()
    {
        return *InterpolationValue;
    }
}
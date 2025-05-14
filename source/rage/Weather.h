#pragma once

#include "Hooking.Patterns.h"

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

    static eWeatherType* CurrentWeather = nullptr;

    bool Init()
    {
        auto pattern = hook::pattern("8B 0D ? ? ? ? 89 08 C3 8B 54 24 04 A1 ? ? ? ?");
        if (!pattern.empty())
        {
            CWeather::CurrentWeather = *(eWeatherType**)pattern.get_first(2);
        }
        else
        {
            return false;
        }

        return true;
    }

    static eWeatherType GetCurrentWeather()
    {
        return *CurrentWeather;
    }
}
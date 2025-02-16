#pragma once

#include "Hooking.Patterns.h"

// From FusionFix
template <size_t count = 1, typename... Args>
hook::pattern find_pattern(Args... args)
{
    hook::pattern pattern;
    ((pattern = hook::pattern(args), !pattern.count_hint(count).empty()) || ...);
    return pattern;
}

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
        auto pattern = find_pattern("A1 ? ? ? ? 83 C4 08 8B CF", "A1 ? ? ? ? 80 3F 04");
        CWeather::CurrentWeather = *pattern.get_first<CWeather::eWeatherType*>(1);

        return true;
    }

    static eWeatherType GetCurrentWeather()
    {
        return *CurrentWeather;
    }
}
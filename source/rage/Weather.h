#pragma once

#include "Hooking.Patterns.h"

// FusionFix code
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
        // FusionFix code, slightly modified to add version detection
        auto pattern = find_pattern("A1 ? ? ? ? 83 C4 08 8B CF", "A1 ? ? ? ? 80 3F 04");
        if (!pattern.empty())
        {
            CWeather::CurrentWeather = *pattern.get_first<CWeather::eWeatherType*>(1);
            return true;
        }
        else
        {
            return false;
        }
    }

    static eWeatherType GetCurrentWeather()
    {
        return *CurrentWeather;
    }
}
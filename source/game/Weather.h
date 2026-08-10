#pragma once

class CWeather
{
public:
    enum eWeatherType : uint32_t
    {
        EXTRASUNNY,
        SUNNY,
        SUNNY_WINDY,
        CLOUDY,
        RAIN,
        DRIZZLE,
        FOGGY,
        LIGHTNING
    };

    static inline eWeatherType* OldWeatherType = nullptr;
    static inline eWeatherType* NewWeatherType = nullptr;
    static inline float* InterpolationValue = nullptr;
};
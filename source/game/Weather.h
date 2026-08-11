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

    static inline eWeatherType* OldWeatherType;
    static inline eWeatherType* NewWeatherType;
    static inline float* InterpolationValue;
};
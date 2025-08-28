#include "Utility.h"

#include "rage/LightSource.h"
#include "rage/Weather.h"

void ReadIni()
{
    CIniReader iniReader("");

    // [VOLUMETRICWEATHERS]
    bExtraSunny = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "ExtraSunny", 0) != 0;
    bSunny      = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Sunny",      0) != 0;
    bSunnyWindy = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "SunnyWindy", 0) != 0;
    bCloudy     = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Cloudy",     0) != 0;
    bRain       = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Rain",       1) != 0;
    bDrizzle    = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Drizzle",    1) != 0;
    bFoggy      = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Foggy",      1) != 0;
    bLightning  = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Lightning",  1) != 0;

    // [SPOTLIGHTS]
    fSpotLightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fSpotLightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fSpotLightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    // [POINTLIGHTS]
    fPointLightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fPointLightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fPointLightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fPointLightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    // [HEADLIGHTS]
    fHeadlightsCoronaSize       = std::clamp(iniReader.ReadFloat("HEADLIGHTS", "HeadlightsCoronaSize", 0.25f), 0.0f, 1.0f);
    fHeadlightsCoronaIntensity  = iniReader.ReadFloat("HEADLIGHTS", "HeadlightsCoronaIntensity", 0.1f);

    // [TAILLIGHTS]
    fTaillightsCoronaSize       = std::clamp(iniReader.ReadFloat("TAILLIGHTS", "TaillightsCoronaSize", 0.25f), 0.0f, 1.0f);
    fTaillightsCoronaIntensity  = iniReader.ReadFloat("TAILLIGHTS", "TaillightsCoronaIntensity", 0.1f);
}

void OnAfterCopyLight(rage::CLightSource*);

// FusionFix code, slightly modified to get rid of the events' stuff
static inline SafetyHookInline shCopyLight{};

static rage::CLightSource* __fastcall CopyLight(void* _this, void* edx, void* a2)
{
    auto ret = shCopyLight.fastcall<rage::CLightSource*>(_this, edx, a2);
    OnAfterCopyLight(ret);

    return ret;
}

bool HasVolumes(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return bExtraSunny;

        case CWeather::SUNNY:
            return bSunny;

        case CWeather::SUNNY_WINDY:
            return bSunnyWindy;

        case CWeather::CLOUDY:
            return bCloudy;

        case CWeather::RAIN:
            return bRain;

        case CWeather::DRIZZLE:
            return bDrizzle;

        case CWeather::FOGGY:
            return bFoggy;

        case CWeather::LIGHTNING:
            return bLightning;
    }
}

float SpotLightVolumeIntensities(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return fSpotLightsVolumeIntensityExtraSunny;

        case CWeather::SUNNY:
            return fSpotLightsVolumeIntensitySunny;

        case CWeather::SUNNY_WINDY:
            return fSpotLightsVolumeIntensitySunnyWindy;

        case CWeather::CLOUDY:
            return fSpotLightsVolumeIntensityCloudy;

        case CWeather::RAIN:
            return fSpotLightsVolumeIntensityRain;

        case CWeather::DRIZZLE:
            return fSpotLightsVolumeIntensityDrizzle;

        case CWeather::FOGGY:
            return fSpotLightsVolumeIntensityFoggy;

        case CWeather::LIGHTNING:
            return fSpotLightsVolumeIntensityLightning;
    }
}

float PointLightVolumeIntensities(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return fPointLightsVolumeIntensityExtraSunny;

        case CWeather::SUNNY:
            return fPointLightsVolumeIntensitySunny;

        case CWeather::SUNNY_WINDY:
            return fPointLightsVolumeIntensitySunnyWindy;

        case CWeather::CLOUDY:
            return fPointLightsVolumeIntensityCloudy;

        case CWeather::RAIN:
            return fPointLightsVolumeIntensityRain;

        case CWeather::DRIZZLE:
            return fPointLightsVolumeIntensityDrizzle;

        case CWeather::FOGGY:
            return fPointLightsVolumeIntensityFoggy;

        case CWeather::LIGHTNING:
            return fPointLightsVolumeIntensityLightning;
    }
}

float SpotLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return fSpotLightsVolumeScaleExtraSunny;

        case CWeather::SUNNY:
            return fSpotLightsVolumeScaleSunny;

        case CWeather::SUNNY_WINDY:
            return fSpotLightsVolumeScaleSunnyWindy;

        case CWeather::CLOUDY:
            return fSpotLightsVolumeScaleCloudy;

        case CWeather::RAIN:
            return fSpotLightsVolumeScaleRain;

        case CWeather::DRIZZLE:
            return fSpotLightsVolumeScaleDrizzle;

        case CWeather::FOGGY:
            return fSpotLightsVolumeScaleFoggy;

        case CWeather::LIGHTNING:
            return fSpotLightsVolumeScaleLightning;
    }
}

float PointLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return fPointLightsVolumeScaleExtraSunny;

        case CWeather::SUNNY:
            return fPointLightsVolumeScaleSunny;

        case CWeather::SUNNY_WINDY:
            return fPointLightsVolumeScaleSunnyWindy;

        case CWeather::CLOUDY:
            return fPointLightsVolumeScaleCloudy;

        case CWeather::RAIN:
            return fPointLightsVolumeScaleRain;

        case CWeather::DRIZZLE:
            return fPointLightsVolumeScaleDrizzle;

        case CWeather::FOGGY:
            return fPointLightsVolumeScaleFoggy;

        case CWeather::LIGHTNING:
            return fPointLightsVolumeScaleLightning;
    }
}

void OnAfterCopyLight(rage::CLightSource *light)
{
    if (HasVolumes(*CWeather::CurrentWeather))
    {
        if (light->mType == rage::LT_SPOT /* Include spotlights */ && light->mProjTexHash == 0xDEAD /* Only include lights "flagged" with LuminescenceHash 57005 */)
        {
            light->mFlags |= 8;
            light->mVolumeIntensity = 4.0f * SpotLightVolumeIntensities(*CWeather::CurrentWeather);
            light->mVolumeScale = SpotLightVolumeScales(*CWeather::CurrentWeather);
        }

        if (light->mType == rage::LT_POINT /* Include pointlights */ && light->mProjTexHash == 0xDEAD /* Only include lights "flagged" with LuminescenceHash 57005 */)
        {
            light->mFlags |= 8;
            light->mVolumeIntensity = 4.0f * PointLightVolumeIntensities(*CWeather::CurrentWeather);
            light->mVolumeScale = PointLightVolumeScales(*CWeather::CurrentWeather);
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        ReadIni();

        if (!CWeather::Init())
        {
            DisplayUnsupportedError();

            return false;
        }

        // CopyLight hook
        {
            // FusionFix code, slightly modified to add version detection
            auto pattern = find_pattern("E8 ? ? ? ? F3 0F 10 44 24 ? 51 F3 0F 11 04 24 56 E8 ? ? ? ? 83 C4 08 FF 05", "E8 ? ? ? ? D9 44 24 0C 51 D9 1C 24 56 E8 ? ? ? ? 83 C4 08");
            if (!pattern.empty())
            {
                shCopyLight = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).get<void*>(), CopyLight);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }
        }

        // Vehicle corona sizes
        {
            auto pattern = hook::pattern("C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? F3 0F 11 04 24 F3 0F 10 44 24 ?");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.count(5).get(0).get<uint32_t>(4), fHeadlightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(1).get<uint32_t>(4), fHeadlightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(2).get<uint32_t>(4), fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(3).get<uint32_t>(4), fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(4).get<uint32_t>(4), fTaillightsCoronaSize, true);
            }
            else
            {
                pattern = hook::pattern("D9 05 ? ? ? ? 83 EC 0C D9 5C 24 08 D9 05 ? ? ? ? D9 5C 24 04 D9 44 24 34");
                injector::WriteMemory(pattern.get_first(2), &fHeadlightsCoronaSize, true);

                pattern = hook::pattern("D9 05 ? ? ? ? 83 EC 0C D9 5C 24 08 8D 54 3E 64 D9 05");
                injector::WriteMemory(pattern.get_first(2), &fHeadlightsCoronaSize, true);

                pattern = hook::pattern("D9 05 ? ? ? ? 83 EC 0C D9 5C 24 08 83 C0 30 D9 05");
                injector::WriteMemory(pattern.count(3).get(0).get<uint32_t>(2), &fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(3).get(1).get<uint32_t>(2), &fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(3).get(2).get<uint32_t>(2), &fTaillightsCoronaSize, true);
            }
        }

        // Vehicle corona intensities
        {
            // Headlights
            auto pattern = hook::pattern("F3 0F 59 15 ? ? ? ? 0A 4D 28");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.get_first(4), &fHeadlightsCoronaIntensity, true);
            }
            else
            {
                pattern = hook::pattern("F3 0F 59 05 ? ? ? ? F3 0F 59 88 ? ? ? ? 88 54 24 0F");
                injector::WriteMemory(pattern.get_first(4), &fHeadlightsCoronaIntensity, true);
            }

            // Taillights
            pattern = hook::pattern("F3 0F 59 15 ? ? ? ? F3 0F 59 88");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.get_first(4), &fTaillightsCoronaIntensity, true);
            }
            else
            {
                pattern = hook::pattern("F3 0F 59 0D ? ? ? ? F3 0F 59 80 ? ? ? ? F3 0F 11 4C 24 ?");
                injector::WriteMemory(pattern.get_first(4), &fTaillightsCoronaIntensity, true);
            }
        }
    }

    return true;
}
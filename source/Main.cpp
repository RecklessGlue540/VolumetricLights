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
    bVolumetricSpotLights      = iniReader.ReadInteger("SPOTLIGHTS", "VolumetricSpotLights", 1) != 0;
    fSpotLightsVolumeIntensity = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensity", 1.0f);
    fSpotLightsVolumeScale     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScale", 0.35f), 0.0f, 0.5f);

    fSpotLightsVolumeScaleSubtractionExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "ExtraSunny", 0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Sunny",      0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SunnyWindy", 0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Cloudy",     0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Rain",       0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Drizzle",    0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Foggy",      0.0f), 0.0f, 0.5f);
    fSpotLightsVolumeScaleSubtractionLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Lightning",  0.0f), 0.0f, 0.5f);

    // [POINTLIGHTS]
    bVolumetricPointLights      = iniReader.ReadInteger("POINTLIGHTS", "VolumetricPointLights", 1) != 0;
    fPointLightsVolumeIntensity = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensity", 0.75f);
    fPointLightsVolumeScale     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScale", 0.2f), 0.0f, 0.35f);

    fPointLightsVolumeScaleSubtractionExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "ExtraSunny", 0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Sunny",      0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SunnyWindy", 0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Cloudy",     0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Rain",       0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Drizzle",    0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Foggy",      0.0f), 0.0f, 0.35f);
    fPointLightsVolumeScaleSubtractionLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Lightning",  0.0f), 0.0f, 0.35f);

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

float SpotLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionExtraSunny;

        case CWeather::SUNNY:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionSunny;

        case CWeather::SUNNY_WINDY:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionSunnyWindy;

        case CWeather::CLOUDY:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionCloudy;

        case CWeather::RAIN:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionRain;

        case CWeather::DRIZZLE:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionDrizzle;

        case CWeather::FOGGY:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionFoggy;

        case CWeather::LIGHTNING:
            return fSpotLightsVolumeScale - fSpotLightsVolumeScaleSubtractionLightning;
    }
}

float PointLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionExtraSunny;

        case CWeather::SUNNY:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionSunny;

        case CWeather::SUNNY_WINDY:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionSunnyWindy;

        case CWeather::CLOUDY:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionCloudy;

        case CWeather::RAIN:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionRain;

        case CWeather::DRIZZLE:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionDrizzle;

        case CWeather::FOGGY:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionFoggy;

        case CWeather::LIGHTNING:
            return fPointLightsVolumeScale - fPointLightsVolumeScaleSubtractionLightning;
    }
}

void OnAfterCopyLight(rage::CLightSource *light)
{
    static CWeather::eWeatherType CurrentWeather;

    CurrentWeather = *CWeather::CurrentWeather;

    if (bVolumetricSpotLights)
    {
        if (light->mType == rage::LT_SPOT /* Include spotlights */ && light->mProjTexHash == 0xDEAD /* Only include lights "flagged" with LuminescenceHash 57005 */)
        {
            if (HasVolumes(CurrentWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fSpotLightsVolumeIntensity;
                light->mVolumeScale = SpotLightVolumeScales(CurrentWeather);
            }
        }
    }

    if (bVolumetricPointLights)
    {
        if (light->mType == rage::LT_POINT /* Include pointlights */ && light->mProjTexHash == 0xDEAD /* Only include lights "flagged" with LuminescenceHash 57005 */)
        {
            if (HasVolumes(CurrentWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fPointLightsVolumeIntensity;
                light->mVolumeScale = PointLightVolumeScales(CurrentWeather);
            }
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
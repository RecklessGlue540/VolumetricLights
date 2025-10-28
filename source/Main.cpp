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
    fSpotlightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fSpotlightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    // [POINTLIGHTS]
    fPointlightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fPointlightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    // [VEHICLELIGHTS]
    fHeadlightsCoronaSize      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "HeadlightsCoronaSize",      0.25f), 0.0f, 1.0f);
    fHeadlightsCoronaIntensity = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "HeadlightsCoronaIntensity", 0.1f),  0.0f, 1.0f);
    fTaillightsCoronaSize      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "TaillightsCoronaSize",      0.25f), 0.0f, 1.0f);
    fTaillightsCoronaIntensity = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "TaillightsCoronaIntensity", 0.1f),  0.0f, 1.0f);
}

bool HasVolumes(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:  return bExtraSunny;
        case CWeather::SUNNY:       return bSunny;
        case CWeather::SUNNY_WINDY: return bSunnyWindy;
        case CWeather::CLOUDY:      return bCloudy;
        case CWeather::RAIN:        return bRain;
        case CWeather::DRIZZLE:     return bDrizzle;
        case CWeather::FOGGY:       return bFoggy;
        case CWeather::LIGHTNING:   return bLightning;
        default: return false;
    }
}

float SpotLightVolumeIntensities(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:  return fSpotlightsVolumeIntensityExtraSunny;
        case CWeather::SUNNY:       return fSpotlightsVolumeIntensitySunny;
        case CWeather::SUNNY_WINDY: return fSpotlightsVolumeIntensitySunnyWindy;
        case CWeather::CLOUDY:      return fSpotlightsVolumeIntensityCloudy;
        case CWeather::RAIN:        return fSpotlightsVolumeIntensityRain;
        case CWeather::DRIZZLE:     return fSpotlightsVolumeIntensityDrizzle;
        case CWeather::FOGGY:       return fSpotlightsVolumeIntensityFoggy;
        case CWeather::LIGHTNING:   return fSpotlightsVolumeIntensityLightning;
        default: return 0.0f;
    }
}

float PointLightVolumeIntensities(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:  return fPointlightsVolumeIntensityExtraSunny;
        case CWeather::SUNNY:       return fPointlightsVolumeIntensitySunny;
        case CWeather::SUNNY_WINDY: return fPointlightsVolumeIntensitySunnyWindy;
        case CWeather::CLOUDY:      return fPointlightsVolumeIntensityCloudy;
        case CWeather::RAIN:        return fPointlightsVolumeIntensityRain;
        case CWeather::DRIZZLE:     return fPointlightsVolumeIntensityDrizzle;
        case CWeather::FOGGY:       return fPointlightsVolumeIntensityFoggy;
        case CWeather::LIGHTNING:   return fPointlightsVolumeIntensityLightning;
        default: return 0.0f;
    }
}

float SpotLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:  return fSpotlightsVolumeScaleExtraSunny;
        case CWeather::SUNNY:       return fSpotlightsVolumeScaleSunny;
        case CWeather::SUNNY_WINDY: return fSpotlightsVolumeScaleSunnyWindy;
        case CWeather::CLOUDY:      return fSpotlightsVolumeScaleCloudy;
        case CWeather::RAIN:        return fSpotlightsVolumeScaleRain;
        case CWeather::DRIZZLE:     return fSpotlightsVolumeScaleDrizzle;
        case CWeather::FOGGY:       return fSpotlightsVolumeScaleFoggy;
        case CWeather::LIGHTNING:   return fSpotlightsVolumeScaleLightning;
        default: return 0.0f;
    }
}

float PointLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
    {
        case CWeather::EXTRASUNNY:  return fPointlightsVolumeScaleExtraSunny;
        case CWeather::SUNNY:       return fPointlightsVolumeScaleSunny;
        case CWeather::SUNNY_WINDY: return fPointlightsVolumeScaleSunnyWindy;
        case CWeather::CLOUDY:      return fPointlightsVolumeScaleCloudy;
        case CWeather::RAIN:        return fPointlightsVolumeScaleRain;
        case CWeather::DRIZZLE:     return fPointlightsVolumeScaleDrizzle;
        case CWeather::FOGGY:       return fPointlightsVolumeScaleFoggy;
        case CWeather::LIGHTNING:   return fPointlightsVolumeScaleLightning;
        default: return 0.0f;
    }
}

// FusionFix code, slightly modified to get rid of the events' stuff
void OnAfterCopyLight(rage::CLightSource*);
static inline SafetyHookInline shCopyLight{};

static rage::CLightSource* __fastcall CopyLight(void* _this, void* edx, void* a2)
{
    auto ret = shCopyLight.fastcall<rage::CLightSource*>(_this, edx, a2);
    OnAfterCopyLight(ret);

    return ret;
}

void OnAfterCopyLight(rage::CLightSource *light)
{
    CWeather::eWeatherType CurrentWeather = CWeather::GetOldWeatherType();
    CWeather::eWeatherType NextWeather  = CWeather::GetNewWeatherType();
    float InterpolationValue = CWeather::GetWeatherInterpolationValue();

    if ((HasVolumes(CurrentWeather) || HasVolumes(NextWeather)) && !IsFusionFixSnowEnabled())
    {
        // Include spotlights and only those "flagged" with "LuminescenceHash" 57005
        if (light->mType == rage::LT_SPOT && light->mProjTexHash == 0xDEAD)
        {
            // Append the "volumetric" light flag
            light->mFlags |= 8;

            // Transition from no volumes to volumes
            if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * SpotLightVolumeIntensities(NextWeather) * InterpolationValue;
                light->mVolumeScale = SpotLightVolumeScales(NextWeather) * InterpolationValue;
            }
            // Transition between volumes
            else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                float CurrentVolumeIntensity = SpotLightVolumeIntensities(CurrentWeather);
                float NextVolumeIntensity = SpotLightVolumeIntensities(NextWeather);

                float CurrentVolumeScale = SpotLightVolumeScales(CurrentWeather);
                float NextVolumeScale = SpotLightVolumeScales(NextWeather);

                light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue);
                light->mVolumeScale = CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue;
            }
            // Transition from volumes to no volumes
            else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * SpotLightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue);
                light->mVolumeScale = SpotLightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
            }
        }

        // Include pointlights and only those "flagged" with "LuminescenceHash" 57005
        if (light->mType == rage::LT_POINT && light->mProjTexHash == 0xDEAD)
        {
            // Append the "volumetric" light flag
            light->mFlags |= 8;

            // Transition from no volumes to volumes
            if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * PointLightVolumeIntensities(NextWeather) * InterpolationValue;
                light->mVolumeScale = PointLightVolumeScales(NextWeather) * InterpolationValue;
            }
            // Transition between volumes
            else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                float CurrentVolumeIntensity = PointLightVolumeIntensities(CurrentWeather);
                float NextVolumeIntensity = PointLightVolumeIntensities(NextWeather);

                float CurrentVolumeScale = PointLightVolumeScales(CurrentWeather);
                float NextVolumeScale = PointLightVolumeScales(NextWeather);

                light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue);
                light->mVolumeScale = CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue;
            }
            // Transition from volumes to no volumes
            else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * PointLightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue);
                light->mVolumeScale = PointLightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
            }
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        ReadIni();

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

        // Weather hooks
        {
            // FusionFix code, slightly modified to add version detection
            auto pattern = find_pattern("A1 ? ? ? ? 83 C4 08 8B CF", "A1 ? ? ? ? 80 3F 04");
            if (!pattern.empty())
            {
                CWeather::OldWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }

            // FusionFix code, slightly modified to add version detection
            pattern = find_pattern("A1 ? ? ? ? 89 46 4C A1", "A1 ? ? ? ? 77 05 A1 ? ? ? ? 80 3F 04");
            if (!pattern.empty())
            {
                CWeather::NewWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }

            // FusionFix code, slightly modified to add version detection
            pattern = hook::pattern("F3 0F 10 05 ? ? ? ? 8B 44 24 0C 8B 4C 24 04");
            if (!pattern.empty())
            {
                CWeather::InterpolationValue = *pattern.get_first<float*>(4);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }
        }

        // Vehicle corona hooks
        {
            // Headlights' and taillights' corona size
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

            // Headlights' corona intensity
            pattern = hook::pattern("F3 0F 59 15 ? ? ? ? 0A 4D 28");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.get_first(4), &fHeadlightsCoronaIntensity, true);
            }
            else
            {
                pattern = hook::pattern("F3 0F 59 05 ? ? ? ? F3 0F 59 88 ? ? ? ? 88 54 24 0F");
                injector::WriteMemory(pattern.get_first(4), &fHeadlightsCoronaIntensity, true);
            }

            // Taillights' corona intensity
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
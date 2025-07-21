#include <Windows.h>

#include "Utils.h"

#include "rage/LightSource.h"
#include "rage/Weather.h"

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only game versions 1.0.7.0 up to 1.2.0.59 are supported.", L"VolumetricLights.asi", MB_ICONERROR | MB_OK);
}

bool bExtraSunny = false;
bool bSunny      = false;
bool bSunnyWindy = false;
bool bCloudy     = false;
bool bRain       = false;
bool bDrizzle    = false;
bool bFoggy      = false;
bool bLightning  = false;

bool  bVolumetricSpotLights      = false;
float fSpotLightsVolumeIntensity = 0.0f;
float fSpotLightsVolumeScale     = 0.0f;

float fSpotLightsVolumeScaleSubtractionExtraSunny = 0.0f;
float fSpotLightsVolumeScaleSubtractionSunny      = 0.0f;
float fSpotLightsVolumeScaleSubtractionSunnyWindy = 0.0f;
float fSpotLightsVolumeScaleSubtractionCloudy     = 0.0f;
float fSpotLightsVolumeScaleSubtractionRain       = 0.0f;
float fSpotLightsVolumeScaleSubtractionDrizzle    = 0.0f;
float fSpotLightsVolumeScaleSubtractionFoggy      = 0.0f;
float fSpotLightsVolumeScaleSubtractionLightning  = 0.0f;

bool  bVolumetricPointLights      = false;
float fPointLightsVolumeIntensity = 0.0f;
float fPointLightsVolumeScale     = 0.0f;

float fPointLightsVolumeScaleSubtractionExtraSunny = 0.0f;
float fPointLightsVolumeScaleSubtractionSunny      = 0.0f;
float fPointLightsVolumeScaleSubtractionSunnyWindy = 0.0f;
float fPointLightsVolumeScaleSubtractionCloudy     = 0.0f;
float fPointLightsVolumeScaleSubtractionRain       = 0.0f;
float fPointLightsVolumeScaleSubtractionDrizzle    = 0.0f;
float fPointLightsVolumeScaleSubtractionFoggy      = 0.0f;
float fPointLightsVolumeScaleSubtractionLightning  = 0.0f;

bool  bVolumetricVehicleLights      = false;
float fVehicleLightsVolumeIntensity = 0.0f;
float fVehicleLightsVolumeScale     = 0.0f;

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
    bVolumetricSpotLights      = iniReader.ReadInteger("SPOTLIGHTS", "VolumetricSpotLights",    1) != 0;
    fSpotLightsVolumeIntensity = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensity", 1.0f);
    fSpotLightsVolumeScale     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScale", 0.4f), 0.0f, 0.5f);

    fSpotLightsVolumeScaleSubtractionExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "ExtraSunny", 0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Sunny",      0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SunnyWindy", 0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Cloudy",     0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Rain",       0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Drizzle",    0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Foggy",      0.0f), 0.0f, 0.3f);
    fSpotLightsVolumeScaleSubtractionLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Lightning",  0.0f), 0.0f, 0.3f);

    // [POINTLIGHTS]
    bVolumetricPointLights      = iniReader.ReadInteger("POINTLIGHTS", "VolumetricPointLights",    1) != 0;
    fPointLightsVolumeIntensity = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensity", 0.75f);
    fPointLightsVolumeScale     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScale", 0.2f), 0.0f, 0.3f);

    fPointLightsVolumeScaleSubtractionExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "ExtraSunny", 0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Sunny",      0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SunnyWindy", 0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Cloudy",     0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Rain",       0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Drizzle",    0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Foggy",      0.0f), 0.0f, 0.2f);
    fPointLightsVolumeScaleSubtractionLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "Lightning",  0.0f), 0.0f, 0.2f);

    // [VEHICLELIGHTS]
    bVolumetricVehicleLights      = iniReader.ReadInteger("VEHICLELIGHTS", "VolumetricVehicleLights",    0) != 0;
    fVehicleLightsVolumeIntensity = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensity", 1.0f);
    fVehicleLightsVolumeScale     = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScale",     0.5f);
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

        default:
            return false;
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

void OnAfterCopyLight(rage::CLightSource*);

// FusionFix code, slightly modified to get rid of the events' stuff
static inline SafetyHookInline shCopyLight{};

static rage::CLightSource* __fastcall CopyLight(void* _this, void* edx, void* a2)
{
    auto ret = shCopyLight.fastcall<rage::CLightSource*>(_this, edx, a2);
    OnAfterCopyLight(ret);
    return ret;
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
                light->mVolumeScale     = SpotLightVolumeScales(CurrentWeather);
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
                light->mVolumeScale     = PointLightVolumeScales(CurrentWeather);
            }
        }
    }

    if (bVolumetricVehicleLights)
    {
        if (light->mType == rage::LT_SPOT /* Include spotlights */ && light->mFlags & 0x100 /* Include vehicle lights */ && !(light->mFlags & 8) /* Exclude helicopter searchlights */)
        {
            if (HasVolumes(CurrentWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fVehicleLightsVolumeIntensity;
                light->mVolumeScale     = fVehicleLightsVolumeScale;
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

    return true;
}
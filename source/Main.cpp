#include <Windows.h>

#include "Utils.h"

#include "rage/LightSource.h"
#include "rage/Weather.h"

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only game versions 1.0.7.0 up to 1.2.0.59 are supported.", L"VolumetricLights.asi", MB_ICONERROR | MB_OK);
}

bool  bVolumetricSpotLights      = false;
float fSpotLightsVolumeIntensity = 0.0f;
float fSpotLightsVolumeScale     = 0.0f;

bool  bVolumetricPointLights      = false;
float fPointLightsVolumeIntensity = 0.0f;
float fPointLightsVolumeScale     = 0.0f;

bool  bVolumetricVehicleLights      = false;
float fVehicleLightsVolumeIntensity = 0.0f;
float fVehicleLightsVolumeScale     = 0.0f;

bool bExtraSunny = false;
bool bSunny      = false;
bool bSunnyWindy = false;
bool bCloudy     = false;
bool bRain       = false;
bool bDrizzle    = false;
bool bFoggy      = false;
bool bLightning  = false;

void ReadIni()
{
    CIniReader iniReader("");

    // [SPOTLIGHTS]
    bVolumetricSpotLights      = iniReader.ReadInteger("SPOTLIGHTS", "VolumetricSpotLights",    1) != 0;
    fSpotLightsVolumeIntensity = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensity", 1.0f);
    fSpotLightsVolumeScale     = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScale",     0.5f);

    // [POINTLIGHTS]
    bVolumetricPointLights      = iniReader.ReadInteger("POINTLIGHTS", "VolumetricPointLights",    1) != 0;
    fPointLightsVolumeIntensity = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensity", 0.75f);
    fPointLightsVolumeScale     = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScale",     0.25f);

    // [VEHICLELIGHTS]
    bVolumetricVehicleLights      = iniReader.ReadInteger("VEHICLELIGHTS", "VolumetricVehicleLights",    0) != 0;
    fVehicleLightsVolumeIntensity = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensity", 1.0f);
    fVehicleLightsVolumeScale     = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScale",     0.5f);

    // [VOLUMETRICWEATHERS]
    bExtraSunny = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "ExtraSunny", 0) != 0;
    bSunny      = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Sunny",      0) != 0;
    bSunnyWindy = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "SunnyWindy", 0) != 0;
    bCloudy     = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Cloudy",     0) != 0;
    bRain       = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Rain",       1) != 0;
    bDrizzle    = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Drizzle",    1) != 0;
    bFoggy      = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Foggy",      1) != 0;
    bLightning  = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Lightning",  1) != 0;
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
        if (light->mProjTexHash == 0xDEAD && light->mType == rage::LT_SPOT)
        {
            if (HasVolumes(CurrentWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fSpotLightsVolumeIntensity;
                light->mVolumeScale = fSpotLightsVolumeScale;
            }
        }
    }

    if (bVolumetricPointLights)
    {
        if (light->mProjTexHash == 0xDEAD && light->mType == rage::LT_POINT)
        {
            if (HasVolumes(CurrentWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fPointLightsVolumeIntensity;
                light->mVolumeScale = fPointLightsVolumeScale;
            }
        }
    }

    if (bVolumetricVehicleLights)
    {
        if (light->mFlags & 0x100 /* Include vehicle lights */ && !(light->mFlags & 8) /* Exclude helicopter searchlights */ && light->mType == rage::LT_SPOT)
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
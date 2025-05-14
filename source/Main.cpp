#include <Windows.h>

#include "Hooking.Patterns.h"
#include "IniReader.h"
#include "injector/injector.hpp"

#include "rage/LightSource.h"
#include "rage/Weather.h"

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only patch 1.0.8.0 is fully supported for now.", L"VolumetricLights.asi", MB_ICONERROR | MB_OK);
}

bool  bVolumetricSpotLights      = false;
float fSpotLightsVolumeIntensity = false;
float fSpotLightsVolumeScale     = false;

bool  bVolumetricPointLights      = false;
float fPointLightsVolumeIntensity = false;
float fPointLightsVolumeScale     = false;

bool  bVolumetricVehicleLights      = false;
float fVehicleLightsVolumeIntensity = false;
float fVehicleLightsVolumeScale     = false;

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

    // [WEATHERS]
    bExtraSunny = iniReader.ReadBoolean("WEATHERS", "ExtraSunny", 0) != 0;
    bSunny      = iniReader.ReadBoolean("WEATHERS", "Sunny",      0) != 0;
    bSunnyWindy = iniReader.ReadBoolean("WEATHERS", "SunnyWindy", 0) != 0;
    bCloudy     = iniReader.ReadBoolean("WEATHERS", "Cloudy",     0) != 0;
    bRain       = iniReader.ReadBoolean("WEATHERS", "Rain",       1) != 0;
    bDrizzle    = iniReader.ReadBoolean("WEATHERS", "Drizzle",    1) != 0;
    bFoggy      = iniReader.ReadBoolean("WEATHERS", "Foggy",      1) != 0;
    bLightning  = iniReader.ReadBoolean("WEATHERS", "Lightning",  1) != 0;
}

void (__cdecl *CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO)() = nullptr;
void (__cdecl *CopyLightO)() = nullptr;
void OnAfterCopyLight(rage::CLightSource*);

void __declspec(naked) CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListH()
{
    _asm
    {
        call CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO

        ret
    }
}

void __declspec(naked) CopyLightH()
{
    _asm
    {
        push [esp+0x4]
        call CopyLightO

        push eax
        call OnAfterCopyLight
        pop eax

        ret 0x4
    }
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
                light->mVolumeScale     = fSpotLightsVolumeScale;
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
                light->mVolumeScale     = fPointLightsVolumeScale;
            }
        }
    }

    if (bVolumetricVehicleLights)
    {
        if (light->mFlags & 0x100 /* Vehicle Lights */ && !(light->mFlags & 8) /* Heli Searchlights */ && light->mType == rage::LT_SPOT)
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

        auto pattern = hook::pattern("C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? C6 46 1C 01 8B C6");
        if (!pattern.empty())
        {
            uintptr_t* vft = *(uintptr_t**)pattern.get_first(2);
            CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO = (void(__cdecl*)())vft[8];
            injector::WriteMemory(&vft[8], CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListH);
        }
        else
        {
            DisplayUnsupportedError();
            return false;
        }

        pattern = hook::pattern("8B CE C1 E1 07 03 0D ? ? ? ? E8 ? ? ? ?");
        if (!pattern.empty())
        {
            CopyLightO = (void(__cdecl*)())injector::GetBranchDestination(pattern.get_first(11)).get();
            injector::MakeCALL(pattern.get_first(11), CopyLightH);
        }
        else
        {
            DisplayUnsupportedError();
            return false;
        }

        pattern = hook::pattern("8B 54 24 08 8B C8 C1 E1 07 03 0D ? ? ? ? 52 E8 ? ? ? ?");
        if (!pattern.empty())
        {
            injector::MakeCALL(pattern.get_first(16), CopyLightH);
        }
        else
        {
            pattern = hook::pattern("FF 74 24 08 C1 E0 07 03 05 ? ? ? ? 8B C8 E8 ? ? ? ? 5E");
            if (!pattern.empty())
            {
                injector::MakeCALL(pattern.get_first(15), CopyLightH);
            }
            else
            {
                DisplayUnsupportedError();
                return false;
            }
        }
    }

    return true;
}
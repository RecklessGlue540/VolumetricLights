#include <Windows.h>

#include "Hooking.Patterns.h"
#include "IniReader.h"
#include "injector/injector.hpp"

#include "rage/LightSource.h"
#include "rage/Weather.h"

void (__cdecl *CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO)() = nullptr;
void (__cdecl *CopyLightO)() = nullptr;
void OnAfterCopyLight(rage::CLightSource*);

void __declspec(naked) CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListH()
{
    _asm
    {
        push ecx

        pop ecx
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
    CIniReader iniReader("");

    // [WEATHERS]
    bool bExtraSunny = iniReader.ReadBoolean("WEATHERS", "ExtraSunny", 0) != 0;
    bool bSunny      = iniReader.ReadBoolean("WEATHERS", "Sunny",      0) != 0;
    bool bSunnyWindy = iniReader.ReadBoolean("WEATHERS", "SunnyWindy", 0) != 0;
    bool bCloudy     = iniReader.ReadBoolean("WEATHERS", "Cloudy",     0) != 0;
    bool bRain       = iniReader.ReadBoolean("WEATHERS", "Rain",       1) != 0;
    bool bDrizzle    = iniReader.ReadBoolean("WEATHERS", "Drizzle",    1) != 0;
    bool bFoggy      = iniReader.ReadBoolean("WEATHERS", "Foggy",      1) != 0;
    bool bLightning  = iniReader.ReadBoolean("WEATHERS", "Lightning",  1) != 0;

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
    CIniReader iniReader("");

    // [SPOTLIGHTS]
    bool bVolumetricSpotLights       = iniReader.ReadInteger("SPOTLIGHTS", "VolumetricSpotLights", 1) != 0;
    float fSpotLightsVolumeIntensity = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensity", 1.0f);
    float fSpotLightsVolumeScale     = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScale", 0.5f);

    // [POINTLIGHTS]
    bool bVolumetricPointLights       = iniReader.ReadInteger("POINTLIGHTS", "VolumetricPointLights", 1) != 0;
    float fPointLightsVolumeIntensity = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensity", 0.85f);
    float fPointLightsVolumeScale     = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScale", 0.25f);

    // [VEHICLELIGHTS]
    bool bVolumetricVehicleLights       = iniReader.ReadInteger("VEHICLELIGHTS", "VolumetricVehicleLights", 0) != 0;
    float fVehicleLightsVolumeIntensity = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensity", 1.0f);
    float fVehicleLightsVolumeScale     = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScale", 0.5f);

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

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only patch 1.0.8.0 is fully supported for now.", L"VolumetricLights.asi", MB_ICONERROR | MB_OK);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        if (!CWeather::Init())
        {
            DisplayUnsupportedError();
            return false;
        }

        // "F3 0F 11 24 24 51 F3 0F 11 84 24 ? ? ? ? F3 0F 11 8C 24 ?" is the closest location for CE, but since the assembly is pretty different this part is beyond me :(
        auto pattern = hook::pattern("D9 1C 24 F3 0F 10 05 ? ? ? ? 8D 4C 24 7C F3 0F 11 54 24 ?");
        if (!pattern.empty())
        {
            uint8_t buffer[] = { 0x44, 0x06, 0xC, 0x90, 0x90 };
            injector::WriteMemoryRaw(pattern.get_first(6), buffer, 5, true);
        }
        else
        {
            DisplayUnsupportedError();
            return false;
        }

        pattern = hook::pattern("C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? C6 46 1C 01 8B C6");
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
            // Needs testing if it's right or not on CE
            pattern = hook::pattern("FF 74 24 08 C1 E0 07 03 05 ? ? ? ? 8B C8 E8 ? ? ? ? 5E");
            if (!pattern.empty())
            {
                injector::MakeCALL(pattern.get_first(12), CopyLightH);
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
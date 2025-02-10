#include <Windows.h>

#include "Hooking.Patterns.h"
#include "IniReader.h"
#include "injector/injector.hpp"

#include "rage/CLightSource.h"
#include "rage/CWeather.h"

bool  bVolumetricSpotLights      = false;
float fSpotLightsVolumeIntensity = 1.0f;
float fSpotLightsVolumeScale     = 0.5f;

bool  bVolumetricPointLights      = false;
float fPointLightsVolumeIntensity = 1.0f;
float fPointLightsVolumeScale     = 0.5f;

bool  bVolumetricVehicleLights      = false;
float fVehicleLightsVolumeIntensity = 1.0f;
float fVehicleLightsVolumeScale     = 0.5f;

bool bExtraSunny = false;
bool bSunny      = false;
bool bSunnyWindy = false;
bool bCloudy     = false;
bool bRain       = false;
bool bDrizzle    = false;
bool bFoggy      = false;
bool bLightning  = false;

bool bLongShadows     = false;
bool bLamppostShadows = false;

void (__cdecl *CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO)() = nullptr;
void (__cdecl *CopyLightO)() = nullptr;

void DisplayUnsupportedError();
void ReadIni();
void OnAfterCopyLight(rage::CLightSource*);

bool HasVolumes(CWeather::eWeatherType type);

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

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only patch 1.0.8.0 is fully supported for now.", L"VolumetricLights.asi", MB_ICONERROR | MB_OK);
}

void ReadIni()
{
    CIniReader iniReader("");

    // [SPOTLIGHTS]
    bVolumetricSpotLights      = iniReader.ReadInteger("SPOTLIGHTS", "VolumetricSpotLights", 1) != 0;
    fSpotLightsVolumeIntensity = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeIntensity", 1.0f);
    fSpotLightsVolumeScale     = iniReader.ReadFloat("SPOTLIGHTS", "SpotLightsVolumeScale", 0.5f);

    // [POINTLIGHTS]
    bVolumetricPointLights      = iniReader.ReadInteger("POINTLIGHTS", "VolumetricPointLights", 1) != 0;
    fPointLightsVolumeIntensity = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeIntensity", 1.0f);
    fPointLightsVolumeScale     = iniReader.ReadFloat("POINTLIGHTS", "PointLightsVolumeScale", 0.5f);

    // [VEHICLELIGHTS]
    bVolumetricVehicleLights      = iniReader.ReadInteger("VEHICLELIGHTS", "VolumetricVehicleLights", 0) != 0;
    fVehicleLightsVolumeIntensity = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensity", 1.0f);
    fVehicleLightsVolumeScale     = iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScale", 0.5f);

    // [WEATHERS]
    bExtraSunny = iniReader.ReadBoolean("WEATHERS", "ExtraSunny", 0) != 0;
    bSunny      = iniReader.ReadBoolean("WEATHERS", "Sunny",      0) != 0;
    bSunnyWindy = iniReader.ReadBoolean("WEATHERS", "SunnyWindy", 0) != 0;
    bCloudy     = iniReader.ReadBoolean("WEATHERS", "Cloudy",     0) != 0;
    bRain       = iniReader.ReadBoolean("WEATHERS", "Rain",       0) != 0;
    bDrizzle    = iniReader.ReadBoolean("WEATHERS", "Drizzle",    0) != 0;
    bFoggy      = iniReader.ReadBoolean("WEATHERS", "Foggy",      1) != 0;
    bLightning  = iniReader.ReadBoolean("WEATHERS", "Lightning",  0) != 0;

    // [MISC]
    bLongShadows     = iniReader.ReadInteger("MISC", "LongShadows",     0) != 0;
    bLamppostShadows = iniReader.ReadInteger("MISC", "LamppostShadows", 1) != 0;
}

void OnAfterCopyLight(rage::CLightSource *light)
{
    static CWeather::eWeatherType CurrWeather;

    CurrWeather = *CWeather::CurrentWeather;

    if (bVolumetricSpotLights)
    {
        if (light->mProjTexHash == 0xDEAD && light->mType == rage::LT_SPOT)
        {
            if (HasVolumes(CurrWeather))
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
            if (HasVolumes(CurrWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fPointLightsVolumeIntensity;
                light->mVolumeScale     = fPointLightsVolumeScale;
            }
        }
    }

    if (bVolumetricVehicleLights)
    {
        if (light->mFlags & 0x100 && light->mType == rage::LT_SPOT)
        {
            if (HasVolumes(CurrWeather))
            {
                light->mFlags |= 8;
                light->mVolumeIntensity = 4.0f * fVehicleLightsVolumeIntensity;
                light->mVolumeScale     = fVehicleLightsVolumeScale;
            }
        }
    }

    if (!bLamppostShadows)
    {
        if (light->mProjTexHash == 0xDEAD)
        {
            light->mFlags &= ~3;
            light->mFlags &= ~4;
        }
    }
}

bool HasVolumes(CWeather::eWeatherType type)
{
    switch(type)
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

        auto pattern = hook::pattern("D9 1C 24 F3 0F 10 05 ? ? ? ? 8D 4C 24 7C F3 0F 11 54 24 ?"); // "F3 0F 11 24 24 51 F3 0F 11 84 24 ? ? ? ? F3 0F 11 8C 24 ?" for CE?
        if (pattern.empty())
        {
            DisplayUnsupportedError();
            return false;
        }
        uint8_t buffer[] = { 0x44, 0x06, 0xC, 0x90, 0x90 };
        injector::WriteMemoryRaw(pattern.get_first(6), buffer, 5, true);

        pattern = hook::pattern("C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? C6 46 1C 01 8B C6"); // Should work with CE
        if (pattern.empty())
        {
            DisplayUnsupportedError();
            return false;
        }
        uintptr_t* vft = *(uintptr_t**)pattern.get_first(2);
        CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO = (void(__cdecl*)())vft[8];
        injector::WriteMemory(&vft[8], CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListH);

        pattern = hook::pattern("8B CE C1 E1 07 03 0D ? ? ? ? E8 ? ? ? ?"); // Should work with CE
        if (pattern.empty())
        {
            DisplayUnsupportedError();
            return false;
        }
        CopyLightO = (void(__cdecl*)())injector::GetBranchDestination(pattern.get_first(11)).get();
        injector::MakeCALL(pattern.get_first(11), CopyLightH);

        pattern = hook::pattern("8B 54 24 08 8B C8 C1 E1 07 03 0D ? ? ? ? 52 E8 ? ? ? ?"); // "FF 74 24 08 C1 E0 07 03 05 ? ? ? ? 8B C8 E8 ? ? ? ? 5E" for CE?
        if (pattern.empty())
        {
            DisplayUnsupportedError();
            return false;
        }
        injector::MakeCALL(pattern.get_first(16), CopyLightH);

        if (bLongShadows)
        {
            auto pattern = hook::pattern("76 ? 0F 2F ? 76 ? 0F 28 ? 0F 28 ? F3 0F 59 ?"); // Works on all patches.
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
        }
    }

    return true;
}
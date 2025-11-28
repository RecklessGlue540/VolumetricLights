#pragma once

#include <Windows.h>

#include "Hooking.Patterns.h"
#include "IniReader.h"
#include "injector/injector.hpp"
#include "injector/assembly.hpp"
#include "safetyhook.hpp"

#include <cstdint>

// FusionFix code
#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

template <size_t count = 1, typename... Args>
hook::pattern find_pattern(Args... args)
{
    hook::pattern pattern;
    ((pattern = hook::pattern(args), !pattern.count_hint(count).empty()) || ...);

    return pattern;
}

typedef bool(__cdecl* FusionFixModeFun)();

bool IsFusionFixModeEnabled(const char* FunctionName)
{
    static HMODULE FusionFix = GetModuleHandleA("GTAIV.EFLC.FusionFix.asi");
    if (!FusionFix)
    {
        return false;
    }

    auto Function = reinterpret_cast<FusionFixModeFun>(GetProcAddress(FusionFix, FunctionName));
    if (!Function)
    {
        return false;
    }

    return Function();
}

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only game versions 1.0.7.0 up to 1.2.0.59 are supported.", L"VolumetricLights.asi", MB_ICONINFORMATION | MB_OK);
}

// Variables
bool bExtraSunny = false;
bool bSunny      = false;
bool bSunnyWindy = false;
bool bCloudy     = false;
bool bRain       = false;
bool bDrizzle    = false;
bool bFoggy      = false;
bool bLightning  = false;

float fSpotlightsVolumeIntensityExtraSunny = 0.0f;
float fSpotlightsVolumeIntensitySunny      = 0.0f;
float fSpotlightsVolumeIntensitySunnyWindy = 0.0f;
float fSpotlightsVolumeIntensityCloudy     = 0.0f;
float fSpotlightsVolumeIntensityRain       = 0.0f;
float fSpotlightsVolumeIntensityDrizzle    = 0.0f;
float fSpotlightsVolumeIntensityFoggy      = 0.0f;
float fSpotlightsVolumeIntensityLightning  = 0.0f;

float fSpotlightsVolumeScaleExtraSunny = 0.0f;
float fSpotlightsVolumeScaleSunny      = 0.0f;
float fSpotlightsVolumeScaleSunnyWindy = 0.0f;
float fSpotlightsVolumeScaleCloudy     = 0.0f;
float fSpotlightsVolumeScaleRain       = 0.0f;
float fSpotlightsVolumeScaleDrizzle    = 0.0f;
float fSpotlightsVolumeScaleFoggy      = 0.0f;
float fSpotlightsVolumeScaleLightning  = 0.0f;

float fPointlightsVolumeIntensityExtraSunny = 0.0f;
float fPointlightsVolumeIntensitySunny      = 0.0f;
float fPointlightsVolumeIntensitySunnyWindy = 0.0f;
float fPointlightsVolumeIntensityCloudy     = 0.0f;
float fPointlightsVolumeIntensityRain       = 0.0f;
float fPointlightsVolumeIntensityDrizzle    = 0.0f;
float fPointlightsVolumeIntensityFoggy      = 0.0f;
float fPointlightsVolumeIntensityLightning  = 0.0f;

float fPointlightsVolumeScaleExtraSunny = 0.0f;
float fPointlightsVolumeScaleSunny      = 0.0f;
float fPointlightsVolumeScaleSunnyWindy = 0.0f;
float fPointlightsVolumeScaleCloudy     = 0.0f;
float fPointlightsVolumeScaleRain       = 0.0f;
float fPointlightsVolumeScaleDrizzle    = 0.0f;
float fPointlightsVolumeScaleFoggy      = 0.0f;
float fPointlightsVolumeScaleLightning  = 0.0f;

float fHeadlightsCoronaSize      = 0.0f;
float fHeadlightsCoronaIntensity = 0.0f;
float fTaillightsCoronaSize      = 0.0f;
float fTaillightsCoronaIntensity = 0.0f;

int iPickupLightsMode = 0;

bool IsFusionFixSnowEnabled()
{
    return IsFusionFixModeEnabled("IsSnowEnabled");
}

bool IsFusionFixHalloweenEnabled()
{
    return IsFusionFixModeEnabled("IsHallEnabled");
}
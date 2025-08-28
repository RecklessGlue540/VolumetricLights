#pragma once

#include <Windows.h>

#include "Hooking.Patterns.h"
#include "IniReader.h"
#include "injector/injector.hpp"
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

void DisplayUnsupportedError()
{
    MessageBox(0, L"Only game versions 1.0.8.0 up to 1.2.0.59 are supported.", L"VolumetricLights.asi", MB_ICONINFORMATION | MB_OK);
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

float fSpotLightsVolumeIntensityExtraSunny = 0.0f;
float fSpotLightsVolumeIntensitySunny      = 0.0f;
float fSpotLightsVolumeIntensitySunnyWindy = 0.0f;
float fSpotLightsVolumeIntensityCloudy     = 0.0f;
float fSpotLightsVolumeIntensityRain       = 0.0f;
float fSpotLightsVolumeIntensityDrizzle    = 0.0f;
float fSpotLightsVolumeIntensityFoggy      = 0.0f;
float fSpotLightsVolumeIntensityLightning  = 0.0f;

float fSpotLightsVolumeScaleExtraSunny = 0.0f;
float fSpotLightsVolumeScaleSunny      = 0.0f;
float fSpotLightsVolumeScaleSunnyWindy = 0.0f;
float fSpotLightsVolumeScaleCloudy     = 0.0f;
float fSpotLightsVolumeScaleRain       = 0.0f;
float fSpotLightsVolumeScaleDrizzle    = 0.0f;
float fSpotLightsVolumeScaleFoggy      = 0.0f;
float fSpotLightsVolumeScaleLightning  = 0.0f;

float fPointLightsVolumeIntensityExtraSunny = 0.0f;
float fPointLightsVolumeIntensitySunny      = 0.0f;
float fPointLightsVolumeIntensitySunnyWindy = 0.0f;
float fPointLightsVolumeIntensityCloudy     = 0.0f;
float fPointLightsVolumeIntensityRain       = 0.0f;
float fPointLightsVolumeIntensityDrizzle    = 0.0f;
float fPointLightsVolumeIntensityFoggy      = 0.0f;
float fPointLightsVolumeIntensityLightning  = 0.0f;

float fPointLightsVolumeScaleExtraSunny = 0.0f;
float fPointLightsVolumeScaleSunny      = 0.0f;
float fPointLightsVolumeScaleSunnyWindy = 0.0f;
float fPointLightsVolumeScaleCloudy     = 0.0f;
float fPointLightsVolumeScaleRain       = 0.0f;
float fPointLightsVolumeScaleDrizzle    = 0.0f;
float fPointLightsVolumeScaleFoggy      = 0.0f;
float fPointLightsVolumeScaleLightning  = 0.0f;

float fHeadlightsCoronaSize      = 0.0f;
float fHeadlightsCoronaIntensity = 0.0f;

float fTaillightsCoronaSize      = 0.0f;
float fTaillightsCoronaIntensity = 0.0f;
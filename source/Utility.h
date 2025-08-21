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

bool  bVolumetricSpotLights = false;
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

bool  bVolumetricPointLights = false;
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

float fHeadlightsCoronaSize = 0.0f;
float fHeadlightsCoronaIntensity = 0.0f;

float fTaillightsCoronaSize = 0.0f;
float fTaillightsCoronaIntensity  = 0.0f;
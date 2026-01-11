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
// TODO

bool IsFusionFixSnowEnabled()
{
    return IsFusionFixModeEnabled("IsSnowEnabled");
}

bool IsFusionFixHalloweenEnabled()
{
    return IsFusionFixModeEnabled("IsHallEnabled");
}
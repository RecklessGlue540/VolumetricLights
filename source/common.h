#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <map>
#include <stacktrace>

#include "Hooking.Patterns.h"
#include "IniReader.h"
#include "injector/assembly.hpp"
#include "injector/utility.hpp"

#include "callbacks.h"

// From: https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/a965bac9437554d8af097220163ac1f29feaa95f/source/comvars.ixx#L10
#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

// From: https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/a965bac9437554d8af097220163ac1f29feaa95f/source/common.ixx#L786
template <size_t count = 1, typename... Args>
hook::pattern find_pattern(Args... args)
{
    hook::pattern pattern;
    ((pattern = hook::pattern(args), !pattern.count_hint(count).empty()) || ...);
    return pattern;
}

// From: https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/a965bac9437554d8af097220163ac1f29feaa95f/source/common.ixx#L455
inline bool IsModuleUAL(HMODULE mod)
{
    if (GetProcAddress(mod, "IsUltimateASILoader") != NULL)
        return true;
    return false;
}

// From: https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/a965bac9437554d8af097220163ac1f29feaa95f/source/common.ixx#L462
bool IsUALPresent()
{
    for (const auto& entry : std::stacktrace::current())
    {
        HMODULE hModule = NULL;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)entry.native_handle(), &hModule))
        {
            if (IsModuleUAL(hModule))
                return true;
        }
    }
    return false;
}

typedef bool(__cdecl* FusionFixEventFun)();

bool IsFusionFixEventEnabled(const char* FunctionName)
{
    static HMODULE FusionFix = GetModuleHandleA("GTAIV.EFLC.FusionFix.asi");

    if (!FusionFix)
        return false;

    auto Function = reinterpret_cast<FusionFixEventFun>(GetProcAddress(FusionFix, FunctionName));

    if (!Function)
        return false;

    return Function();
}

bool IsFusionFixSnowEventEnabled()
{
    return IsFusionFixEventEnabled("IsSnowEnabled");
}

bool IsFusionFixHalloweenEventEnabled()
{
    return IsFusionFixEventEnabled("IsHallEnabled");
}
#pragma once

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
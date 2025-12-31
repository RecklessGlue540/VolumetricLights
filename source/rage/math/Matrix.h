#pragma once

#include "Vector.h"

// From https://github.com/akifle47/fxdc/blob/main/src/rage/math/Matrix.h

namespace rage
{
    class Matrix44
    {
    public:
        Vector4 a;
        Vector4 b;
        Vector4 c;
        Vector4 d;
    };
}
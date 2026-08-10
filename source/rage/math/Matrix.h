// From: https://github.com/akifle47/ptfxwiz/blob/main/src/rage/math/Matrix.h

#pragma once

#include "Vector.h"

namespace rage
{
    class Matrix44
    {
    public:
        Matrix44() {}
        Matrix44(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d) : a(a), b(b), c(c), d(d) {}

        Vector4 a;
        Vector4 b;
        Vector4 c;
        Vector4 d;
    };

    static const Matrix44 gMatrix44Identity {{1.0f, 0.0f, 0.0f, 0.0f},
                                             {0.0f, 1.0f, 0.0f, 0.0f},
                                             {0.0f, 0.0f, 1.0f, 0.0f},
                                             {0.0f, 0.0f, 0.0f, 1.0f}};

    class Matrix34
    {
    public:
        Matrix34() {}
        Matrix34(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d) : a(a), b(b), c(c), d(d) {}

        Vector4 a;
        Vector4 b;
        Vector4 c;
        Vector4 d;
    };

    static const Matrix34 gMatrix34Identity{{1.0f, 0.0f, 0.0f, 0.0f},
                                            {0.0f, 1.0f, 0.0f, 0.0f},
                                            {0.0f, 0.0f, 1.0f, 0.0f},
                                            {0.0f, 0.0f, 0.0f, 1.0f}};
}
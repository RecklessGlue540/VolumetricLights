#pragma once

#include "Vector.h"

#include <cstdint>

namespace rage
{
    enum eLightType
    {
        LT_POINT   = 0x0,
        LT_DIR     = 0x1,
        LT_SPOT    = 0x2,
        LT_AMBOCC  = 0x3,
        LT_CLAMPED = 0x4,
    };

    class CLightSource
    {
    public:
        Vector3 mDirection;     // Direction
        float field_C;
        Vector3 mTangent;       // Origin
        float field_1C;
        Vector3 mPosition;      // Position
        float field_2C;
        Vector4 mColor;         // Color
        float mIntensity;       // Range
        eLightType mType;       // Type
        uint32_t mFlags;        // Flags
        int32_t mTxdId;         // CoronaHash
        int32_t mProjTexHash;   // LuminescenceHash
        float mRadius;          // Distance
        float mInnerConeAngle;  // HotSpot
        float mOuterConeAngle;  // Falloff
        int32_t field_60;
        int32_t field_64;
        int32_t field_68;
        int32_t field_6C;
        float mVolumeIntensity; // _f2C
        float mVolumeScale;     // _f30
        int8_t field_78[7];
        char field_7F;
    };

    static_assert(sizeof(CLightSource) == 0x80);
}
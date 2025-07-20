#pragma once

#include "../Utils.h"

#include "Vector.h"

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
        int32_t mCoronaTexHash; // CoronaHash
        int32_t mProjTexHash;   // LuminescenceHash
        float mRadius;          // Distance
        float mInnerConeAngle;  // HotSpot
        float mOuterConeAngle;  // Falloff
        int32_t field_60;
        int32_t field_64;
        int32_t field_68;
        int32_t field_6C;
        float mVolumeIntensity; // This one is a bit weird, it affects volume intensity exactly the same way as the _f2C from light models, yet it's different
        float mVolumeScale;     // _f30
        int8_t field_78[7];
        char field_7F;
    };

    VALIDATE_SIZE(CLightSource, 0x80);
}
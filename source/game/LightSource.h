#pragma once

#include "rage/math/Vector.h"

enum eLightType
{
    LT_POINT,
    LT_DIR,
    LT_SPOT,
    LT_AMBOCC,
    LT_CLAMPED
};

enum eLightFlags
{
    LIGHTFLAG_CAST_STATIC_GEOM_SHADOWS  = (1 << 1),
    LIGHTFLAG_CAST_DYNAMIC_GEOM_SHADOWS = (1 << 2),
    LIGHTFLAG_DRAW_VOLUME               = (1 << 3),
    LIGHTFLAG_NO_SPECULAR               = (1 << 4),
    LIGHTFLAG_INTERIOR_ONLY             = (1 << 5), // Probably
    LIGHTFLAG_EXTERIOR_ONLY             = (1 << 6), // Probably
    LIGHTFLAG_CUTSCENE                  = (1 << 7), // CutsceneObject lights
    LIGHTFLAG_VEHICLE                   = (1 << 8),
    LIGHTFLAG_TRAFFIC_LIGHT             = (1 << 9),
    LIGHTFLAG_USE_VEHICLE_TWIN          = (1 << 10) // PC exclusive, used for that headlight shadow madness
};

class CLightSource
{
public:
    rage::Vector3 m_Direction;
    float m_Padding1;
    rage::Vector3 m_Tangent;
    float m_Padding2;
    rage::Vector3 m_Position;
    float m_Padding3;
    rage::Vector4 m_Color;
    float m_Intensity;
    eLightType m_LightType;
    uint32_t m_Flags;
    uint32_t m_LightOccluderTxdSlot; // Likely
    uint32_t m_ProjectedTextureNameKey;
    float m_Radius;
    float m_InnerConeAngle;
    float m_OuterConeAngle;
    int32_t m_CastShadows;
    int32_t m_ShadowCacheIndex;
    int32_t m_InteriorIndex;
    int32_t m_RoomIndex;
    float m_VolumeIntensity;
    float m_VolumeScale;
    int8_t field_78[7];
    char field_7F;
};

VALIDATE_SIZE(CLightSource, 0x80);
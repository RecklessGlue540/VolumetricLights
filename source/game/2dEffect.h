#pragma once

#include "rage/math/Vector.h"

enum eLightAttrFlags
{
    LIGHTATTRFLAG_DAY_ONLY                  = (1 << 5),
    LIGHTATTRFLAG_NIGHT_ONLY                = (1 << 6),
    LIGHTATTRFLAG_CAST_STATIC_GEOM_SHADOWS  = (1 << 14),
    LIGHTATTRFLAG_CAST_DYNAMIC_GEOM_SHADOWS = (1 << 15),
    LIGHTATTRFLAG_CALC_FROM_SUN             = (1 << 16),
    LIGHTATTRFLAG_ENABLE_BUZZING            = (1 << 17),
    LIGHTATTRFLAG_FORCE_BUZZING             = (1 << 18),
    LIGHTATTRFLAG_DRAW_VOLUME               = (1 << 19),
    LIGHTATTRFLAG_NO_SPECULAR               = (1 << 20)
};

class C2dEffect
{
public:
    virtual ~C2dEffect() = default;

    rage::Vector3 m_Position;
};

VALIDATE_SIZE(C2dEffect, 0x10);

class CLightAttr : public C2dEffect
{
public:
    rage::Vector3 m_Direction;
    rage::Vector3 m_Tangent;
    uint8_t m_Color[3];
    uint8_t m_Flashiness;
    float m_CoronaFadeDistance; // Unused
    float m_VolumeIntensity;
    float m_VolumeScale;
    float m_Radius;
    float m_Intensity;
    float m_CoronaSize;
    float m_InnerConeAngle;
    float m_OuterConeAngle;
    uint32_t m_Flags;
    uint32_t m_CoronaTextureNameHash;
    uint32_t m_ProjectedTextureNameHash;
    uint32_t m_TimeFlags : 24;
    uint32_t m_LightType : 8;
    float m_CoronaIntensity;
    float m_LightFadeDistance;
    float m_VolumeFadeDistance;
    int16_t m_BoneTag;
    int16_t m_Padding; // Probably
};

VALIDATE_SIZE(CLightAttr, 0x6C);
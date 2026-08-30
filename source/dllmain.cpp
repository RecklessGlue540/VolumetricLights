#include "common.h"

#include "game/2dEffect.h"
#include "game/LightSource.h"
#include "game/ModelInfo.h"
#include "game/Weather.h"

#include "rage/StringHash.h"
#include "rage/math/Matrix.h"

class CFogVolumes
{
public:
    // Members
    static inline bool ms_bWeatherHasVolumes[8];
    static inline float ms_fSpotlightVolumeIntensity[8];
    static inline float ms_fSpotlightVolumeScale[8];
    static inline float ms_fPointlightVolumeIntensity[8];
    static inline float ms_fPointlightVolumeScale[8];
    static inline float ms_fVehicleLightVolumeIntensity[8];
    static inline float ms_fVehicleLightVolumeScale[8];

    // Technically these should be added as new members to CLightAttr, copying how that class extender thing from FF works.
    // Since we don't use modules here and it would be all too tedious for such a minor thing, we just clear the map later in CLightAttr's destructor.
    static inline std::unordered_map<CLightAttr*, std::pair<float, float>> ms_BaseModelVolumeParams;

    static inline bool GetVolumesEnabledForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_bWeatherHasVolumes[WeatherType];
    }

    static inline float GetSpotlightVolumeIntensityForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_fSpotlightVolumeIntensity[WeatherType];
    }

    static inline float GetSpotlightVolumeScaleForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_fSpotlightVolumeScale[WeatherType];
    }

    static inline float GetPointlightVolumeIntensityForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_fPointlightVolumeIntensity[WeatherType];
    }

    static inline float GetPointlightVolumeScaleForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_fPointlightVolumeScale[WeatherType];
    }

    static inline float GetVehicleLightVolumeIntensityForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_fVehicleLightVolumeIntensity[WeatherType];
    }

    static inline float GetVehicleLightVolumeScaleForWeather(CWeather::eWeatherType WeatherType)
    {
        return ms_fVehicleLightVolumeScale[WeatherType];
    }

    static inline float ms_fSpotlightInnerConeAngle;
    static inline float ms_fSpotlightOuterConeAngle;

    static inline float ms_fSpotlightCoronaSize;
    static inline float ms_fSpotlightCoronaIntensity;

    static inline float ms_fPointlightCoronaSize;
    static inline float ms_fPointlightCoronaIntensity;

    static inline float ms_fLightFadeDistance;
    static inline float ms_fVolumeFadeDistance;

    static inline bool ms_bUnfakeVehicleLights;
    static inline bool ms_bUnfakeVehicleSirenLights;
    static inline bool ms_bEnableVehicleLightVolumes;

    static inline bool ms_bDebugUseSunLights;
    static inline bool ms_bDebugUseFillLights;

    // Methods
    static void LoadConfigFile();
    static void AddFogVolume(CLightAttr* pLightAttr, float (*VolumeIntensity)(CWeather::eWeatherType), float (*VolumeScale)(CWeather::eWeatherType));
    static void Update(CLightAttr* pLightAttr);
};

void CFogVolumes::LoadConfigFile()
{
    CIniReader iniReader("");

    // [Weathers]
    ms_bWeatherHasVolumes[CWeather::EXTRASUNNY]  = iniReader.ReadInteger("Weathers", "ExtraSunnyHasVolumes", 0) != 0;
    ms_bWeatherHasVolumes[CWeather::SUNNY]       = iniReader.ReadInteger("Weathers", "SunnyHasVolumes",      0) != 0;
    ms_bWeatherHasVolumes[CWeather::SUNNY_WINDY] = iniReader.ReadInteger("Weathers", "SunnyWindyHasVolumes", 0) != 0;
    ms_bWeatherHasVolumes[CWeather::CLOUDY]      = iniReader.ReadInteger("Weathers", "CloudyHasVolumes",     0) != 0;
    ms_bWeatherHasVolumes[CWeather::RAIN]        = iniReader.ReadInteger("Weathers", "RainHasVolumes",       1) != 0;
    ms_bWeatherHasVolumes[CWeather::DRIZZLE]     = iniReader.ReadInteger("Weathers", "DrizzleHasVolumes",    1) != 0;
    ms_bWeatherHasVolumes[CWeather::FOGGY]       = iniReader.ReadInteger("Weathers", "FoggyHasVolumes",      1) != 0;
    ms_bWeatherHasVolumes[CWeather::LIGHTNING]   = iniReader.ReadInteger("Weathers", "LightningHasVolumes",  1) != 0;

    // [Spotlights]
    ms_fSpotlightVolumeIntensity[CWeather::EXTRASUNNY]  = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensityExtraSunny", 0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::SUNNY]       = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensitySunny",      0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::SUNNY_WINDY] = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::CLOUDY]      = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensityCloudy",     0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::RAIN]        = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensityRain",       0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::DRIZZLE]     = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensityDrizzle",    0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::FOGGY]       = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensityFoggy",      0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeIntensity[CWeather::LIGHTNING]   = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeIntensityLightning",  0.0f), 0.0f, 1.0f);

    ms_fSpotlightVolumeScale[CWeather::EXTRASUNNY]  = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleExtraSunny", 0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::SUNNY]       = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleSunny",      0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::SUNNY_WINDY] = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleSunnyWindy", 0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::CLOUDY]      = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleCloudy",     0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::RAIN]        = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleRain",       0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::DRIZZLE]     = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleDrizzle",    0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::FOGGY]       = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleFoggy",      0.0f), 0.0f, 1.0f);
    ms_fSpotlightVolumeScale[CWeather::LIGHTNING]   = std::clamp(iniReader.ReadFloat("Spotlights", "VolumeScaleLightning",  0.0f), 0.0f, 1.0f);

    ms_fSpotlightInnerConeAngle = iniReader.ReadFloat("Spotlights", "InnerConeAngle", -1.0f);
    ms_fSpotlightOuterConeAngle = iniReader.ReadFloat("Spotlights", "OuterConeAngle", -1.0f);

    ms_fSpotlightCoronaSize      = iniReader.ReadFloat("Spotlights", "CoronaSize",      -1.0f);
    ms_fSpotlightCoronaIntensity = iniReader.ReadFloat("Spotlights", "CoronaIntensity", -1.0f);

    // [Pointlights]
    ms_fPointlightVolumeIntensity[CWeather::EXTRASUNNY]  = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensityExtraSunny", 0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::SUNNY]       = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensitySunny",      0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::SUNNY_WINDY] = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::CLOUDY]      = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensityCloudy",     0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::RAIN]        = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensityRain",       0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::DRIZZLE]     = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensityDrizzle",    0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::FOGGY]       = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensityFoggy",      0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeIntensity[CWeather::LIGHTNING]   = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeIntensityLightning",  0.0f), 0.0f, 1.0f);

    ms_fPointlightVolumeScale[CWeather::EXTRASUNNY]  = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleExtraSunny", 0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::SUNNY]       = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleSunny",      0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::SUNNY_WINDY] = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleSunnyWindy", 0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::CLOUDY]      = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleCloudy",     0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::RAIN]        = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleRain",       0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::DRIZZLE]     = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleDrizzle",    0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::FOGGY]       = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleFoggy",      0.0f), 0.0f, 1.0f);
    ms_fPointlightVolumeScale[CWeather::LIGHTNING]   = std::clamp(iniReader.ReadFloat("Pointlights", "VolumeScaleLightning",  0.0f), 0.0f, 1.0f);

    ms_fPointlightCoronaSize      = iniReader.ReadFloat("Pointlights", "CoronaSize",      -1.0f);
    ms_fPointlightCoronaIntensity = iniReader.ReadFloat("Pointlights", "CoronaIntensity", -1.0f);

    // [Shared]
    ms_fLightFadeDistance  = iniReader.ReadFloat("Shared", "LightFadeDistance",  -1.0f);
    ms_fVolumeFadeDistance = iniReader.ReadFloat("Shared", "VolumeFadeDistance", -1.0f);

    // [VehicleLights]
    ms_bUnfakeVehicleLights       = iniReader.ReadInteger("VehicleLights", "UnfakeLights",      0) != 0;
    ms_bUnfakeVehicleSirenLights  = iniReader.ReadInteger("VehicleLights", "UnfakeSirenLights", 0) != 0;
    ms_bEnableVehicleLightVolumes = iniReader.ReadInteger("VehicleLights", "EnableVolumes",     0) != 0;

    ms_fVehicleLightVolumeIntensity[CWeather::EXTRASUNNY]  = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensityExtraSunny", 0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::SUNNY]       = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensitySunny",      0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::SUNNY_WINDY] = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::CLOUDY]      = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensityCloudy",     0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::RAIN]        = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensityRain",       0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::DRIZZLE]     = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensityDrizzle",    0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::FOGGY]       = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensityFoggy",      0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeIntensity[CWeather::LIGHTNING]   = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeIntensityLightning",  0.0f), 0.0f, 1.0f);

    ms_fVehicleLightVolumeScale[CWeather::EXTRASUNNY]  = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleExtraSunny", 0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::SUNNY]       = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleSunny",      0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::SUNNY_WINDY] = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleSunnyWindy", 0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::CLOUDY]      = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleCloudy",     0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::RAIN]        = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleRain",       0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::DRIZZLE]     = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleDrizzle",    0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::FOGGY]       = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleFoggy",      0.0f), 0.0f, 1.0f);
    ms_fVehicleLightVolumeScale[CWeather::LIGHTNING]   = std::clamp(iniReader.ReadFloat("VehicleLights", "VolumeScaleLightning",  0.0f), 0.0f, 1.0f);

    // [Debug]
    ms_bDebugUseSunLights  = iniReader.ReadInteger("Debug", "DebugUseSunLights",  0) != 0;
    ms_bDebugUseFillLights = iniReader.ReadInteger("Debug", "DebugUseFilllights", 0) != 0;
}

void CFogVolumes::AddFogVolume(CLightAttr* pLightAttr, float (*VolumeIntensity)(CWeather::eWeatherType), float (*VolumeScale)(CWeather::eWeatherType))
{
    const CWeather::eWeatherType OldWeather = *CWeather::OldWeatherType;
    const CWeather::eWeatherType NewWeather = *CWeather::NewWeatherType;
    const float InterpolationValue = *CWeather::InterpolationValue;

    // Capture the original model volume properties only once, so that we can add to them later
    const auto It = CFogVolumes::ms_BaseModelVolumeParams.emplace(pLightAttr, std::make_pair(pLightAttr->m_VolumeIntensity, pLightAttr->m_VolumeScale)).first;

    const float BaseModelVolumeIntensity = It->second.first;
    const float BaseModelVolumeScale = It->second.second;

    float ExtraVolumeIntensity = 0.0f;
    float ExtraVolumeScale = 0.0f;

    pLightAttr->m_Flags |= LIGHTATTRFLAG_DRAW_VOLUME;

    // Handle compatibility with FusionFix's snow volumes, just let it do its thing
    if (IsFusionFixSnowEventEnabled())
    {
        pLightAttr->m_Flags &= ~LIGHTATTRFLAG_DRAW_VOLUME;
    }
    // Transition between no volumes to volumes
    else if (!GetVolumesEnabledForWeather(OldWeather) && GetVolumesEnabledForWeather(NewWeather))
    {
        ExtraVolumeIntensity = VolumeIntensity(NewWeather) * InterpolationValue;
        ExtraVolumeScale = VolumeScale(NewWeather) * InterpolationValue;
    }
    // Transition between volumes
    else if (GetVolumesEnabledForWeather(OldWeather) && GetVolumesEnabledForWeather(NewWeather))
    {
        ExtraVolumeIntensity = std::lerp(VolumeIntensity(OldWeather), VolumeIntensity(NewWeather), InterpolationValue);
        ExtraVolumeScale = std::lerp(VolumeScale(OldWeather), VolumeScale(NewWeather), InterpolationValue);
    }
    // Transition between volumes to no volumes
    else if (GetVolumesEnabledForWeather(OldWeather) && !GetVolumesEnabledForWeather(NewWeather))
    {
        ExtraVolumeIntensity = VolumeIntensity(OldWeather) * (1.0f - InterpolationValue);
        ExtraVolumeScale = VolumeScale(OldWeather) * (1.0f - InterpolationValue);
    }
    else
    {
        pLightAttr->m_Flags &= ~LIGHTATTRFLAG_DRAW_VOLUME;
    }

    // Add custom values on top of the volume properties set in models, this allows having some more control over the volume properties where needed
    pLightAttr->m_VolumeIntensity = BaseModelVolumeIntensity + ExtraVolumeIntensity;
    pLightAttr->m_VolumeScale = BaseModelVolumeScale + ExtraVolumeScale;
}

void CFogVolumes::Update(CLightAttr* pLightAttr)
{
    // Custom "flag" to check LightAttrs against, this makes sure we're not affecting lights we shouldn't,
    // so whatever we want to affect needs to be edited manually to support this.
    // This can also be beneficial because we can provide improvements to the models that we have to supply anyways.
    //
    // The bad part is that anything done under this flag will be done for all flagged models, only separated by whether a light contains spotlights or pointlights
    // (E.g. custom cone angles for spotlight models would override the cone angles of all affected models at once, which may not look accurate on all models).
    if (pLightAttr->m_ProjectedTextureNameKey != 0xDEAD)
        return;

    // Spot
    if (pLightAttr->m_LightType == 2)
    {
        AddFogVolume(pLightAttr, GetSpotlightVolumeIntensityForWeather, GetSpotlightVolumeScaleForWeather);

        // Custom cone angles, if -1.0, don't override
        {
            if (ms_fSpotlightInnerConeAngle != -1.0f)
                pLightAttr->m_InnerConeAngle = ms_fSpotlightInnerConeAngle;

            if (ms_fSpotlightOuterConeAngle != -1.0f)
                pLightAttr->m_OuterConeAngle = ms_fSpotlightOuterConeAngle;
        }

        // Custom corona properties, if -1.0, don't override
        {
            if (ms_fSpotlightCoronaSize != -1.0f)
                pLightAttr->m_CoronaSize = ms_fSpotlightCoronaSize;

            if (ms_fSpotlightCoronaIntensity != -1.0f)
                pLightAttr->m_CoronaIntensity = ms_fSpotlightCoronaIntensity;
        }
    }
    // Point
    else if (pLightAttr->m_LightType == 1)
    {
        AddFogVolume(pLightAttr, GetPointlightVolumeIntensityForWeather, GetPointlightVolumeScaleForWeather);

        // Custom corona properties, if -1.0, don't override
        {
            if (ms_fPointlightCoronaSize != -1.0f)
                pLightAttr->m_CoronaSize = ms_fPointlightCoronaSize;

            if (ms_fPointlightCoronaIntensity != -1.0f)
                pLightAttr->m_CoronaIntensity = ms_fPointlightCoronaIntensity;
        }
    }

    // Shared
    {
        // Custom fade values
        {
            pLightAttr->m_LightFadeDistance = ms_fLightFadeDistance;
            pLightAttr->m_VolumeFadeDistance = ms_fVolumeFadeDistance;
        }

        // Debug flags
        {
            if (ms_bDebugUseSunLights)
                pLightAttr->m_Flags |= LIGHTATTRFLAG_CALC_FROM_SUN;

            if (ms_bDebugUseFillLights)
                pLightAttr->m_Flags |= LIGHTATTRFLAG_NO_SPECULAR;
        }
    }
}

SafetyHookInline shProcessOne2dEffectLight = {};
void __cdecl ProcessOne2dEffectLight(CLightAttr* pLightAttr, rage::Matrix34* a2, float a3, float a4, float a5, uint16_t a6, float a7, int a8, int a9, int a10, char a11, char a12, char a13)
{
    CFogVolumes::Update(pLightAttr);

    shProcessOne2dEffectLight.unsafe_ccall(pLightAttr, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

SafetyHookInline shCLightAttr__dtor = {};
CLightAttr* __fastcall CLightAttr__dtor(CLightAttr* _this, void* edx, int a2)
{
    CFogVolumes::ms_BaseModelVolumeParams.erase(_this);

    return shCLightAttr__dtor.unsafe_fastcall<CLightAttr*>(_this, edx, a2);
}

void Init()
{
    CFogVolumes::LoadConfigFile();

    hook::pattern pattern;

    // Extend ProcessOne2dEffectLight to pass the LightAttr object pointer in order to modify its properties at will
    pattern = find_pattern("55 8B EC 83 E4 ? 81 EC ? ? ? ? 56 8B 75 ? 32 C0", "55 8B EC 83 E4 ? 83 EC ? 8B 4D ? F3 0F 10 05 ? ? ? ? 53 56");
    shProcessOne2dEffectLight = safetyhook::create_inline(pattern.get_first(0), ProcessOne2dEffectLight);
    
    // Erase original volume parameter map objects when LightAttr objects get destroyed
    pattern = hook::pattern("56 8B F1 8B 4C 24 ? F6 C1 ? 74 ? 8B 56 ? 57 8D 7E ? 8B C2 6B C0");
    if (!pattern.count(4).empty())
    {
        shCLightAttr__dtor = safetyhook::create_inline(pattern.count(4).get(2).get<void*>(0), CLightAttr__dtor);
    }
    else
    {
        pattern = hook::pattern("8A 54 24 ? F6 C2 ? 56 8B F1 74 ? 8B 4E ? 57 8D 7E ? 8B C1 6B C0");
        shCLightAttr__dtor = safetyhook::create_inline(pattern.count(3).get(1).get<void*>(0), CLightAttr__dtor);
    }

    // Weather variables
    pattern = find_pattern("8B 3D ? ? ? ? 8B 0D ? ? ? ? F3 0F 11 0D", "8B 35 ? ? ? ? 8B 0D ? ? ? ? 0F 28 E0");
    CWeather::OldWeatherType = *pattern.get_first<CWeather::eWeatherType*>(2);
    CWeather::NewWeatherType = *pattern.get_first<CWeather::eWeatherType*>(8);

    pattern = find_pattern("F3 0F 10 05 ? ? ? ? F3 0F 59 CA 0F 2F C1 F3 0F 11 4C 24", "F3 0F 10 0D ? ? ? ? 0F 2F C8 76 ? 8B 0D ? ? ? ? 85 C9");
    CWeather::InterpolationValue = *pattern.get_first<float*>(4);

    // The ParticleAttr limit requires an increase so TBoGT doesn't poof with all the added particle effects to lights
    pattern = hook::pattern("83 C4 ? B9 ? ? ? ? A3 ? ? ? ? E8 ? ? ? ? B9");
    auto gParticleStore2 = *pattern.get_first<CModelInfo::CStaticStore*>(19);

    // We check if the array size is vanilla and only then increase it by two, to ensure we don't interfere with other limit adjusters here.
    if (gParticleStore2->nSize == 0x0A8C) // 2700
    {
        gParticleStore2->nSize *= 2;
    }
}

extern "C"
{
    void __declspec(dllexport) InitializeASI()
    {
        std::call_once(CallbackHandler::flag, []()
        {
            CallbackHandler::RegisterCallbackAtGetSystemTimeAsFileTime(Init, find_pattern("83 EC ? A1 ? ? ? ? 89 04 24", "83 EC ? A1 ? ? ? ? 8B 0D ? ? ? ? 6A"));
        });
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (!IsUALPresent()) { InitializeASI(); }
    }

    return TRUE;
}
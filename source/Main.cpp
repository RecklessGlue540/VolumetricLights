#include "Utility.h"

#include "game/ModelInfoStore.h"
#include "game/Weather.h"
#include "rage/LightSource.h"
#include "rage/StringHash.h"
#include "rage/math/Matrix.h"

static void OnAfterCopyLight(rage::CLightSource*);

// From https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/e3daeaa774106fcc8a0c4decf4d6710f49c311d8/source/comvars.ixx#L2100
static inline SafetyHookInline shCopyLight{};

static rage::CLightSource* __fastcall CopyLight(void* _this, void* edx, void* a2)
{
    auto ret = shCopyLight.fastcall<rage::CLightSource*>(_this, edx, a2);
    OnAfterCopyLight(ret);

    return ret;
}

static void(__stdcall* GET_ROOT_CAM)(int* Camera);
static void(__cdecl* GET_CAM_POS)(int Camera, float* PositionX, float* PositionY, float* PositionZ);

static float* dwViewDistance = nullptr;

typedef	void* (__cdecl* AddSingleVehicleLight_T)(rage::Matrix44* TransformationMatrix, float* Position, rage::Vector3* Direction, rage::Vector3* Color, float Intensity, float Radius, float InnerConeAngle, float OuterConeAngle, int InteriorIndex, int RoomIndex, int ShadowCacheIndex, char a12, char a13);
AddSingleVehicleLight_T AddSingleVehicleLight = nullptr;

static uintptr_t ResumeHeadlights = 0;
static uintptr_t ResumeTaillights = 0;
static uintptr_t ResumeReverselights = 0;

static float* dwInnerConeAngle = nullptr;
static float* dwOuterConeAngle = nullptr;

void ReadSettings()
{
    CIniReader iniReader("");

    // [WEATHERS]
    bExtraSunny = iniReader.ReadInteger("WEATHERS", "ExtraSunny", 0) != 0;
    bSunny      = iniReader.ReadInteger("WEATHERS", "Sunny",      0) != 0;
    bSunnyWindy = iniReader.ReadInteger("WEATHERS", "SunnyWindy", 0) != 0;
    bCloudy     = iniReader.ReadInteger("WEATHERS", "Cloudy",     0) != 0;
    bRain       = iniReader.ReadInteger("WEATHERS", "Rain",       1) != 0;
    bDrizzle    = iniReader.ReadInteger("WEATHERS", "Drizzle",    1) != 0;
    bFoggy      = iniReader.ReadInteger("WEATHERS", "Foggy",      1) != 0;
    bLightning  = iniReader.ReadInteger("WEATHERS", "Lightning",  1) != 0;

    // [SPOTLIGHTS]
    fSpotlightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fSpotlightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fSpotlightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fSpotlightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    fSpotlightsVolumeFadeStart = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeFadeStart", 75.0f),  0.0f, 300.0f);
    fSpotlightsVolumeFadeEnd   = std::clamp(iniReader.ReadFloat("SPOTLIGHTS", "SpotlightsVolumeFadeEnd",   150.0f), 0.0f, 300.0f);

    // [POINTLIGHTS]
    fPointlightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fPointlightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fPointlightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fPointlightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    fPointlightsVolumeFadeStart = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeFadeStart", 75.0f),  0.0f, 300.0f);
    fPointlightsVolumeFadeEnd   = std::clamp(iniReader.ReadFloat("POINTLIGHTS", "PointlightsVolumeFadeEnd",   150.0f), 0.0f, 300.0f);

    // [VEHICLELIGHTS]
    bDualVehicleLights = iniReader.ReadInteger("VEHICLELIGHTS", "DualVehicleLights", 0) != 0;

    bVolumetricVehicleLights = iniReader.ReadInteger("VEHICLELIGHTS", "VolumetricVehicleLights", 0) != 0;

    fVehicleLightsVolumeIntensityExtraSunny = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensityExtraSunny", 0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensitySunny      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensitySunny",      0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensitySunnyWindy = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensitySunnyWindy", 0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensityCloudy     = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensityCloudy",     0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensityRain       = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensityRain",       0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensityDrizzle    = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensityDrizzle",    0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensityFoggy      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensityFoggy",      0.0f), 0.0f, 1.5f);
    fVehicleLightsVolumeIntensityLightning  = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeIntensityLightning",  0.0f), 0.0f, 1.5f);

    fVehicleLightsVolumeScaleExtraSunny = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleExtraSunny", 0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleSunny      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleSunny",      0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleSunnyWindy = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleSunnyWindy", 0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleCloudy     = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleCloudy",     0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleRain       = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleRain",       0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleDrizzle    = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleDrizzle",    0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleFoggy      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleFoggy",      0.0f), 0.0f, 0.5f);
    fVehicleLightsVolumeScaleLightning  = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeScaleLightning",  0.0f), 0.0f, 0.5f);

    fVehicleLightsVolumeFadeStart = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeFadeStart", 25.0f), 0.0f, 100.0f);
    fVehicleLightsVolumeFadeEnd   = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "VehicleLightsVolumeFadeEnd",   50.0f), 0.0f, 100.0f);

    fHeadlightsCoronaSize = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "HeadlightsCoronaSize", 0.25f), 0.0f, 1.0f);
    fTaillightsCoronaSize = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "TaillightsCoronaSize", 0.25f), 0.0f, 1.0f);

    fHeadlightsCoronaIntensity = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "HeadlightsCoronaIntensity", 0.1f), 0.0f, 1.0f);
    fTaillightsCoronaIntensity = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "TaillightsCoronaIntensity", 0.1f), 0.0f, 1.0f);

    // [PICKUPLIGHTS]
    iPickupLightsMode = iniReader.ReadInteger("PICKUPLIGHTS", "PickupLightsMode", 1);
}

bool HasVolumes(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return bExtraSunny;
        case CWeather::SUNNY:       return bSunny;
        case CWeather::SUNNY_WINDY: return bSunnyWindy;
        case CWeather::CLOUDY:      return bCloudy;
        case CWeather::RAIN:        return bRain;
        case CWeather::DRIZZLE:     return bDrizzle;
        case CWeather::FOGGY:       return bFoggy;
        case CWeather::LIGHTNING:   return bLightning;
        default: return false;
    }
}

float SpotlightVolumeIntensities(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return fSpotlightsVolumeIntensityExtraSunny;
        case CWeather::SUNNY:       return fSpotlightsVolumeIntensitySunny;
        case CWeather::SUNNY_WINDY: return fSpotlightsVolumeIntensitySunnyWindy;
        case CWeather::CLOUDY:      return fSpotlightsVolumeIntensityCloudy;
        case CWeather::RAIN:        return fSpotlightsVolumeIntensityRain;
        case CWeather::DRIZZLE:     return fSpotlightsVolumeIntensityDrizzle;
        case CWeather::FOGGY:       return fSpotlightsVolumeIntensityFoggy;
        case CWeather::LIGHTNING:   return fSpotlightsVolumeIntensityLightning;
        default: return 0.0f;
    }
}

float PointlightVolumeIntensities(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return fPointlightsVolumeIntensityExtraSunny;
        case CWeather::SUNNY:       return fPointlightsVolumeIntensitySunny;
        case CWeather::SUNNY_WINDY: return fPointlightsVolumeIntensitySunnyWindy;
        case CWeather::CLOUDY:      return fPointlightsVolumeIntensityCloudy;
        case CWeather::RAIN:        return fPointlightsVolumeIntensityRain;
        case CWeather::DRIZZLE:     return fPointlightsVolumeIntensityDrizzle;
        case CWeather::FOGGY:       return fPointlightsVolumeIntensityFoggy;
        case CWeather::LIGHTNING:   return fPointlightsVolumeIntensityLightning;
        default: return 0.0f;
    }
}

float SpotlightVolumeScales(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return fSpotlightsVolumeScaleExtraSunny;
        case CWeather::SUNNY:       return fSpotlightsVolumeScaleSunny;
        case CWeather::SUNNY_WINDY: return fSpotlightsVolumeScaleSunnyWindy;
        case CWeather::CLOUDY:      return fSpotlightsVolumeScaleCloudy;
        case CWeather::RAIN:        return fSpotlightsVolumeScaleRain;
        case CWeather::DRIZZLE:     return fSpotlightsVolumeScaleDrizzle;
        case CWeather::FOGGY:       return fSpotlightsVolumeScaleFoggy;
        case CWeather::LIGHTNING:   return fSpotlightsVolumeScaleLightning;
        default: return 0.0f;
    }
}

float PointlightVolumeScales(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return fPointlightsVolumeScaleExtraSunny;
        case CWeather::SUNNY:       return fPointlightsVolumeScaleSunny;
        case CWeather::SUNNY_WINDY: return fPointlightsVolumeScaleSunnyWindy;
        case CWeather::CLOUDY:      return fPointlightsVolumeScaleCloudy;
        case CWeather::RAIN:        return fPointlightsVolumeScaleRain;
        case CWeather::DRIZZLE:     return fPointlightsVolumeScaleDrizzle;
        case CWeather::FOGGY:       return fPointlightsVolumeScaleFoggy;
        case CWeather::LIGHTNING:   return fPointlightsVolumeScaleLightning;
        default: return 0.0f;
    }
}

float VehicleLightVolumeIntensities(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return fVehicleLightsVolumeIntensityExtraSunny;
        case CWeather::SUNNY:       return fVehicleLightsVolumeIntensitySunny;
        case CWeather::SUNNY_WINDY: return fVehicleLightsVolumeIntensitySunnyWindy;
        case CWeather::CLOUDY:      return fVehicleLightsVolumeIntensityCloudy;
        case CWeather::RAIN:        return fVehicleLightsVolumeIntensityRain;
        case CWeather::DRIZZLE:     return fVehicleLightsVolumeIntensityDrizzle;
        case CWeather::FOGGY:       return fVehicleLightsVolumeIntensityFoggy;
        case CWeather::LIGHTNING:   return fVehicleLightsVolumeIntensityLightning;
        default: return 0.0f;
    }
}

float VehicleLightVolumeScales(CWeather::eWeatherType Type)
{
    switch (Type)
    {
        case CWeather::EXTRASUNNY:  return fVehicleLightsVolumeScaleExtraSunny;
        case CWeather::SUNNY:       return fVehicleLightsVolumeScaleSunny;
        case CWeather::SUNNY_WINDY: return fVehicleLightsVolumeScaleSunnyWindy;
        case CWeather::CLOUDY:      return fVehicleLightsVolumeScaleCloudy;
        case CWeather::RAIN:        return fVehicleLightsVolumeScaleRain;
        case CWeather::DRIZZLE:     return fVehicleLightsVolumeScaleDrizzle;
        case CWeather::FOGGY:       return fVehicleLightsVolumeScaleFoggy;
        case CWeather::LIGHTNING:   return fVehicleLightsVolumeScaleLightning;
        default: return 0.0f;
    }
}

void OnAfterCopyLight(rage::CLightSource* light)
{
    const CWeather::eWeatherType CurrentWeather = CWeather::GetOldWeatherType();
    const CWeather::eWeatherType NextWeather = CWeather::GetNewWeatherType();
    const float InterpolationValue = CWeather::GetWeatherInterpolationValue();

    int CurrentCamera;
    rage::Vector3 CameraPosition;
    GET_ROOT_CAM(&CurrentCamera);
    GET_CAM_POS(CurrentCamera, &CameraPosition.x, &CameraPosition.y, &CameraPosition.z);

    static auto Smoothstep = [](float Edge0, float Edge1, float X)
    {
        float NormalizedX = std::clamp((X - Edge0) / (Edge1 - Edge0), 0.0f, 1.0f);

        return NormalizedX * NormalizedX * (3.0f - 2.0f * NormalizedX);
    };

    if ((HasVolumes(CurrentWeather) || HasVolumes(NextWeather)) && !IsFusionFixSnowEnabled())
    {
        // Include spotlights and only those "flagged" with "LuminescenceHash" 57005
        if (light->mType == rage::LT_SPOT && light->mProjTexHash == 0xDEAD)
        {
            // Append the light shaft flag
            light->mFlags |= 8;

            // Distance fading setup
            float DeltaX = CameraPosition.x - light->mPosition.x;
            float DeltaY = CameraPosition.y - light->mPosition.y;
            float DeltaZ = CameraPosition.z - light->mPosition.z;

            float Distance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);

            float FadeStart = fSpotlightsVolumeFadeStart * *dwViewDistance;
            float FadeEnd = fSpotlightsVolumeFadeEnd * *dwViewDistance;

            float DistanceFade = 1.0f - Smoothstep(FadeStart, FadeEnd, Distance);

            // Transition from no volumes to volumes
            if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * SpotlightVolumeIntensities(NextWeather) * InterpolationValue * DistanceFade;
                light->mVolumeScale = SpotlightVolumeScales(NextWeather) * InterpolationValue;
            }
            // Transition between volumes
            else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                float CurrentVolumeIntensity = SpotlightVolumeIntensities(CurrentWeather);
                float NextVolumeIntensity = SpotlightVolumeIntensities(NextWeather);

                float CurrentVolumeScale = SpotlightVolumeScales(CurrentWeather);
                float NextVolumeScale = SpotlightVolumeScales(NextWeather);

                light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue) * DistanceFade;
                light->mVolumeScale = (CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue);
            }
            // Transition from volumes to no volumes
            else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * SpotlightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue) * DistanceFade;
                light->mVolumeScale = SpotlightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
            }
        }

        // Include pointlights and only those "flagged" with "LuminescenceHash" 57005
        if (light->mType == rage::LT_POINT && light->mProjTexHash == 0xDEAD)
        {
            // Append the light shaft flag
            light->mFlags |= 8;

            // Distance fading setup
            float DeltaX = CameraPosition.x - light->mPosition.x;
            float DeltaY = CameraPosition.y - light->mPosition.y;
            float DeltaZ = CameraPosition.z - light->mPosition.z;

            float Distance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);

            float FadeStart = fPointlightsVolumeFadeStart * *dwViewDistance;
            float FadeEnd = fPointlightsVolumeFadeEnd * *dwViewDistance;

            float DistanceFade = 1.0f - Smoothstep(FadeStart, FadeEnd, Distance);

            // Transition from no volumes to volumes
            if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * PointlightVolumeIntensities(NextWeather) * InterpolationValue * DistanceFade;
                light->mVolumeScale = PointlightVolumeScales(NextWeather) * InterpolationValue;
            }
            // Transition between volumes
            else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                float CurrentVolumeIntensity = PointlightVolumeIntensities(CurrentWeather);
                float NextVolumeIntensity = PointlightVolumeIntensities(NextWeather);

                float CurrentVolumeScale = PointlightVolumeScales(CurrentWeather);
                float NextVolumeScale = PointlightVolumeScales(NextWeather);

                light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue) * DistanceFade;
                light->mVolumeScale = (CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue);
            }
            // Transition from volumes to no volumes
            else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * PointlightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue) * DistanceFade;
                light->mVolumeScale = PointlightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
            }
        }

        if (bVolumetricVehicleLights && bDualVehicleLights /* We need dual vehicle lights for proper light shaft positions */ )
        {
            // Include spotlights, only vehicle lights, and exclude lights previously volumetric (Mainly helicopter searchlights)
            if (light->mType == rage::LT_SPOT && light->mFlags & 0x100 && !(light->mFlags & 8))
            {
                // Append the light shaft flag
                light->mFlags |= 8;

                // Distance fading setup
                float DeltaX = CameraPosition.x - light->mPosition.x;
                float DeltaY = CameraPosition.y - light->mPosition.y;
                float DeltaZ = CameraPosition.z - light->mPosition.z;

                float Distance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);

                float FadeStart = fVehicleLightsVolumeFadeStart;
                float FadeEnd = fVehicleLightsVolumeFadeEnd;

                float DistanceFade = 1.0f - Smoothstep(FadeStart, FadeEnd, Distance);

                // Transition from no volumes to volumes
                if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
                {
                    light->mVolumeIntensity = 4.0f * VehicleLightVolumeIntensities(NextWeather) * InterpolationValue * DistanceFade;
                    light->mVolumeScale = VehicleLightVolumeScales(NextWeather) * InterpolationValue;
                }
                // Transition between volumes
                else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
                {
                    float CurrentVolumeIntensity = VehicleLightVolumeIntensities(CurrentWeather);
                    float NextVolumeIntensity = VehicleLightVolumeIntensities(NextWeather);

                    float CurrentVolumeScale = VehicleLightVolumeScales(CurrentWeather);
                    float NextVolumeScale = VehicleLightVolumeScales(NextWeather);

                    light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue) * DistanceFade;
                    light->mVolumeScale = (CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue);
                }
                // Transition from volumes to no volumes
                else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
                {
                    light->mVolumeIntensity = 4.0f * VehicleLightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue) * DistanceFade;
                    light->mVolumeScale = VehicleLightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
                }
            }
        }
    }
}

void __cdecl RenderCenterHeadlight(rage::Matrix44* TransformationMatrix, rage::Matrix44* LeftPosition, rage::Matrix44* RightPosition, float* Position, rage::Vector3* Direction, rage::Vector3* Color, float Intensity, float Radius, int64_t a9, int InteriorIndex, int RoomIndex, int ShadowCacheIndex, char a13)
{
    // We multiply by the game dwords at the end there which also get updated upon changing the cone angles in visualsettings.dat :) (To be clear, not at runtime)
    float InnerConeAngle = 0.8f * (1.0f * *dwInnerConeAngle);
    float OuterConeAngle = 0.8f * (1.0f * *dwOuterConeAngle);

    AddSingleVehicleLight(TransformationMatrix, &LeftPosition->d.x,  Direction, Color, Intensity, Radius, InnerConeAngle, OuterConeAngle, InteriorIndex, RoomIndex, ShadowCacheIndex + 1 /* Without this the left light will not be able to cast a shadow */, 1, a13);
    AddSingleVehicleLight(TransformationMatrix, &RightPosition->d.x, Direction, Color, Intensity, Radius, InnerConeAngle, OuterConeAngle, InteriorIndex, RoomIndex, ShadowCacheIndex, 1, a13);
}

void RenderCenterTaillight(rage::Matrix44* TransformationMatrix, rage::Matrix44* LeftPosition, rage::Matrix44* RightPosition, rage::Vector3* Direction, rage::Vector3* Color, float Intensity, float Radius, float InnerConeAngle, float OuterConeAngle, int InteriorIndex, int RoomIndex, int ShadowCacheIndex, char a13, char a14)
{
    AddSingleVehicleLight(TransformationMatrix, &LeftPosition->d.x,  Direction, Color, Intensity, Radius, InnerConeAngle * 0.6f, OuterConeAngle * 0.6f, InteriorIndex, RoomIndex, ShadowCacheIndex, a13, a14);
    AddSingleVehicleLight(TransformationMatrix, &RightPosition->d.x, Direction, Color, Intensity, Radius, InnerConeAngle * 0.6f, OuterConeAngle * 0.6f, InteriorIndex, RoomIndex, ShadowCacheIndex, a13, a14);
}

void __declspec(naked) RenderCenterHeadlightStub()
{
    __asm
    {
        mov ecx, [esp + 0x50]
        mov eax, [esp + 0x54]

        push ecx
        push eax

        push dword ptr[ebp + 0x24]

        call RenderCenterHeadlight
        add esp, 0x38

        mov eax, ResumeHeadlights
        add eax, 11
        jmp eax
    }
}

void __declspec(naked) RenderCenterTaillightStub()
{
    __asm
    {
        mov ecx, [esp + 0x58]
        mov eax, [esp + 0x50]

        push ecx
        push eax

        push dword ptr[ebp + 0x2C]

        call RenderCenterTaillight
        add esp, 0x38

        mov eax, ResumeTaillights
        add eax, 16
        jmp eax
    }
}

// TODO: Reverselights
/*void RenderCenterReverselight(rage::Matrix44* TransformationMatrix, rage::Matrix44* LeftPosition, rage::Matrix44* RightPosition, rage::Vector3* Direction, rage::Vector3* Color, float Intensity, float Radius, float InnerConeAngle, float OuterConeAngle, int InteriorIndex, int RoomIndex, int ShadowCacheIndex, char a13, char a14)
{
    AddSingleVehicleLight(TransformationMatrix, &LeftPosition->d.x,  Direction, Color, Intensity, Radius, InnerConeAngle, OuterConeAngle, InteriorIndex, RoomIndex, ShadowCacheIndex, a13, a14);
    AddSingleVehicleLight(TransformationMatrix, &RightPosition->d.x, Direction, Color, Intensity, Radius, InnerConeAngle, OuterConeAngle, InteriorIndex, RoomIndex, ShadowCacheIndex, a13, a14);
}

void __declspec(naked) RenderCenterReverselightStub()
{
    __asm
    {
        // idk anymore
        mov ecx, [esp + 0x??]
        mov eax, [esp + 0x??]

        push ecx
        push eax

        push dword ptr[ebp + 0x20]

        call RenderCenterReverselight
        add esp, 0x38

        mov eax, ResumeReverselights
        add eax, 16
        jmp eax
    }
}*/

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        ReadSettings();

        // Main hooks
        {
            // CopyLight hook
            // (#1 https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/722e4a056f72e2b6fe39f2b34f14a3d37fa03919/source/comvars.ixx#L2556)
            {
                auto pattern = find_pattern("E8 ? ? ? ? F3 0F 10 44 24 ? 51 F3 0F 11 04 24 56 E8 ? ? ? ? 83 C4 08 FF 05", "E8 ? ? ? ? D9 44 24 0C 51 D9 1C 24 56 E8 ? ? ? ? 83 C4 08");
                if (!pattern.empty())
                {
                    shCopyLight = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).get<void*>(), CopyLight);
                }
            }

            // Weather hooks
            // (#1 https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/722e4a056f72e2b6fe39f2b34f14a3d37fa03919/source/comvars.ixx#L2567)
            // (#2 https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/722e4a056f72e2b6fe39f2b34f14a3d37fa03919/source/comvars.ixx#L2570)
            // (#3 https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/722e4a056f72e2b6fe39f2b34f14a3d37fa03919/source/comvars.ixx#L2573)
            {
                auto pattern = find_pattern("A1 ? ? ? ? 83 C4 08 8B CF", "A1 ? ? ? ? 80 3F 04");
                if (!pattern.empty())
                {
                    CWeather::OldWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
                }

                pattern = find_pattern("A1 ? ? ? ? 89 46 4C A1", "A1 ? ? ? ? 77 05 A1 ? ? ? ? 80 3F 04");
                if (!pattern.empty())
                {
                    CWeather::NewWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
                }

                pattern = hook::pattern("F3 0F 10 05 ? ? ? ? 8B 44 24 0C 8B 4C 24 04");
                if (!pattern.empty())
                {
                    CWeather::InterpolationValue = *pattern.get_first<float*>(4);
                }
            }

            // Camera native hooks
            // (#1: https://github.com/ThirteenAG/III.VC.SA.IV.Project2DFX/blob/068178563aed28ee51dc2d2e2f2afec61f804f8f/source/IVLodLights/dllmain.cpp#L278)
            // (#2: https://github.com/ThirteenAG/III.VC.SA.IV.Project2DFX/blob/068178563aed28ee51dc2d2e2f2afec61f804f8f/source/IVLodLights/dllmain.cpp#L284)
            {
                auto pattern = find_pattern("FF 35 ? ? ? ? 8B 0D ? ? ? ? E8 ? ? ? ? 8B 4C 24 04 89 01 C2 04 00 CC", "A1 ? ? ? ? 8B 0D ? ? ? ? 50 E8 ? ? ? ? 8B 4C 24 04 89 01 C2 04 00 CC");
                if (!pattern.empty())
                {
                    GET_ROOT_CAM = (void(__stdcall*)(int* Camera))(pattern.get_first(0));
                }

                pattern = find_pattern("55 8B EC 83 E4 F0 83 EC 10 8D 04 24 50 FF 75 08", "55 8B EC 83 E4 F0 8B 4D 08 83 EC 10 8D 04 24 50 51");
                if (!pattern.empty())
                {
                    GET_CAM_POS = (void(__cdecl*)(int Camera, float* PositionX, float* PositionY, float* PositionZ))(pattern.get(0).get<uintptr_t>(0));
                }
            }

            // View distance slider hook
            {
                auto pattern = find_pattern("F3 0F 11 05 ? ? ? ? 66 0F 6E 05 ? ? ? ? 0F 5B C0 F3 0F 59 05 ? ? ? ? F3 0F 59 05 ? ? ? ? F3 0F 58 05 ? ? ? ? F3 0F 11 04 24", "F3 0F 11 05 ? ? ? ? F3 0F 2A 05 ? ? ? ? F3 0F 59 05 ? ? ? ? F3 0F 58 05 ? ? ? ? D3 E6");
                if (!pattern.empty())
                {
                    dwViewDistance = *pattern.get_first<float*>(4);
                }
            }

            // Limit hooks
            // (#1 https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/722e4a056f72e2b6fe39f2b34f14a3d37fa03919/source/limits.ixx#L421)
            // (#2 https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/c3ce288433bb8be081b0cf80b0af8d5f9f14e733/source/limits.ixx#L159)
            {
                // The ParticleAttr limit requires an increase so TBoGT doesn't poof with all the provided .ide files which add a bunch of particles
                auto pattern = hook::pattern("8B C8 E8 ? ? ? ? B9 ? ? ? ? A3");
                if (!pattern.empty())
                {
                    auto CModelInfoStore__ms_baseModels = *pattern.get_first<CModelInfoStore::CDataStore*>(8);

                    // We check if the array size is vanilla and only then increase it by two, to ensure we don't interfere with other limit adjusters here
                    if (CModelInfoStore__ms_baseModels[CModelInfoStore::ms_particleAttrs].nSize == 0x0A8C)
                    {
                        CModelInfoStore__ms_baseModels[CModelInfoStore::ms_particleAttrs].nSize *= 2;
                    }
                }
            }

            // TODO: Dual vehicle light hooks (Thanks to @xoxor4d (https://github.com/xoxor4d))
            {
                if (bDualVehicleLights)
                {
                    // Single light function
                    auto pattern = find_pattern("55 8B EC 83 E4 F0 83 EC 20 80 7D 34 00 8B 4D 08 8B 45 10", "55 8B EC 83 E4 F0 83 EC 2C 80 7D 24 00 D9 45 1C 8B 45 08 F3 0F 10 31");
                    if (!pattern.empty())
                    {
                        AddSingleVehicleLight = (AddSingleVehicleLight_T)pattern.get_first(0);
                    }

                    // Headlights
                    {
                        auto pattern = hook::pattern("FF 75 24 E8 ? ? ? ? 83 C4 30 5F 5E 8B E5 5D C2 24 00");
                        if (!pattern.empty())
                        {
                            injector::MakeNOP(pattern.get_first(0), 8, true);
                            injector::MakeJMP(pattern.get_first(0), RenderCenterHeadlightStub, true);
                            uintptr_t BaseAddressHeadlights = (uintptr_t)pattern.get_first(0);
                            ResumeHeadlights = BaseAddressHeadlights;
                        }
                        // TODO: preCE

                        // Right light position override?
                        pattern = hook::pattern("F3 0F 11 44 24 ? E8 ? ? ? ? 8D 44 24 60 50 8B 44 24 24 50 8B CE");
                        if (!pattern.empty())
                        {
                            injector::MakeNOP(pattern.get_first(0), 6, true);
                        }

                        // Right light position override read? No idea if needed or what it even does :)
                        pattern = hook::pattern("F3 0F 10 74 24 ? F3 0F 59 25 ? ? ? ? F3 0F 11 74 24 ? F3 0F 59 2D ? ? ? ? F3 0F 11 64 24 ? 6A 00");
                        injector::MakeNOP(pattern.get_first(0), 6, true);

                        // Cone angle dwords
                        pattern = hook::pattern("F3 0F 59 15 ? ? ? ? F3 0F C2 C3 06 F3 0F 10 1D ? ? ? ? F3 0F 59 1D ? ? ? ? F3 0F 11 54 24 ? 0F 54 C6");
                        if (!pattern.empty())
                        {
                            dwInnerConeAngle = *pattern.get_first<float*>(4);
                            dwOuterConeAngle = *pattern.get_first<float*>(25);
                        }
                    }

                    // Taillights
                    {
                        auto pattern = hook::pattern("8D 44 24 6C 50 FF 75 2C E8 ? ? ? ? 83 C4 34 5F 5E 8B E5 5D C2 2C 00");
                        if (!pattern.empty())
                        {
                            injector::MakeJMP(pattern.get_first(0), RenderCenterTaillightStub, true);
                            uintptr_t BaseAddressTaillights = (uintptr_t)pattern.get_first(0);
                            ResumeTaillights = BaseAddressTaillights;
                        }
                        // TODO: preCE
                    }

                    // TODO: Reverselights
                    /*{
                        auto pattern = hook::pattern("8D 44 24 ? 50 FF 75 ? E8 ? ? ? ? 83 C4 ? 5F 5E 8B E5 5D C2");
                        injector::MakeJMP(pattern.get_first(0), RenderCenterReverselightStub, true);
                        uintptr_t BaseAddressReverselights = (uintptr_t)pattern.get_first(0);
                        ResumeReverselights = BaseAddressReverselights;
                    }*/
                }
            }
        }

        // Vehicle corona hooks
        {
            // Headlights' and taillights' corona size
            auto pattern = hook::pattern("C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? F3 0F 11 04 24 F3 0F 10 44 24 ?");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.count(5).get(0).get<uint32_t>(4), fHeadlightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(1).get<uint32_t>(4), fHeadlightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(2).get<uint32_t>(4), fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(3).get<uint32_t>(4), fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(5).get(4).get<uint32_t>(4), fTaillightsCoronaSize, true);
            }
            else
            {
                pattern = hook::pattern("D9 05 ? ? ? ? 83 EC 0C D9 5C 24 08 D9 05 ? ? ? ? D9 5C 24 04 D9 44 24 34");
                injector::WriteMemory(pattern.get_first(2), &fHeadlightsCoronaSize, true);

                pattern = hook::pattern("D9 05 ? ? ? ? 83 EC 0C D9 5C 24 08 8D 54 3E 64 D9 05");
                injector::WriteMemory(pattern.get_first(2), &fHeadlightsCoronaSize, true);

                pattern = hook::pattern("D9 05 ? ? ? ? 83 EC 0C D9 5C 24 08 83 C0 30 D9 05");
                injector::WriteMemory(pattern.count(3).get(0).get<uint32_t>(2), &fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(3).get(1).get<uint32_t>(2), &fTaillightsCoronaSize, true);
                injector::WriteMemory(pattern.count(3).get(2).get<uint32_t>(2), &fTaillightsCoronaSize, true);
            }

            // Headlights' corona intensity
            pattern = hook::pattern("F3 0F 59 15 ? ? ? ? 0A 4D 28");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.get_first(4), &fHeadlightsCoronaIntensity, true);
            }
            else
            {
                pattern = hook::pattern("F3 0F 59 05 ? ? ? ? F3 0F 59 88 ? ? ? ? 88 54 24 0F");
                injector::WriteMemory(pattern.get_first(4), &fHeadlightsCoronaIntensity, true);
            }

            // Taillights' corona intensity
            pattern = hook::pattern("F3 0F 59 15 ? ? ? ? F3 0F 59 88");
            if (!pattern.empty())
            {
                injector::WriteMemory(pattern.get_first(4), &fTaillightsCoronaIntensity, true);
            }
            else
            {
                pattern = hook::pattern("F3 0F 59 0D ? ? ? ? F3 0F 59 80 ? ? ? ? F3 0F 11 4C 24 ?");
                injector::WriteMemory(pattern.get_first(4), &fTaillightsCoronaIntensity, true);
            }
        }

        // Pickup light hooks
        {
            if (iPickupLightsMode == 0)
            {
                auto pattern = find_pattern("F3 0F 11 04 24 50 57 8D 44 24 4C 50", "F3 0F 11 04 24 8D 4C 24 34 51 57 8D 54 24 4C 52 8D 44 24 60 50");
                if (!pattern.empty())
                {
                    struct PickupLightsHook
                    {
                        void operator()(injector::reg_pack& regs)
                        {
                            regs.xmm0.f32[0] = 0.0f; // Null intensity
                        }
                    }; injector::MakeInline<PickupLightsHook>(pattern.get_first(0));
                }
            }
            else if (iPickupLightsMode == 2)
            {
                auto pattern = hook::pattern("68 ? ? ? ? 6A 00 6A 00 C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? E8 ? ? ? ? F3 0F 10 54 24 ? 46");
                if (!pattern.empty())
                {
                    injector::WriteMemory(pattern.get_first(1), 0xD3, true); // push 201 --> push 211 (+Fill lighting)
                }
                else
                {
                    pattern = hook::pattern("68 ? ? ? ? 6A 00 6A 00 F3 0F 11 54 24 ? E8 ? ? ? ? 83 C6 01 83 C4 40 83 C7 10");
                    injector::WriteMemory(pattern.get_first(1), 0xD3, true); // push 201 --> push 211 (+Fill lighting)
                }
            }
        }
    }

    return true;
}
#include "Utility.h"

#include "rage/LightSource.h"
#include "rage/Matrix.h"
#include "rage/ModelInfoStore.h"
#include "rage/Vector.h"
#include "rage/Weather.h"

void ReadIni()
{
    CIniReader iniReader("");

    // [VOLUMETRICWEATHERS]
    bExtraSunny = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "ExtraSunny", 0) != 0;
    bSunny      = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Sunny",      0) != 0;
    bSunnyWindy = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "SunnyWindy", 0) != 0;
    bCloudy     = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Cloudy",     0) != 0;
    bRain       = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Rain",       1) != 0;
    bDrizzle    = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Drizzle",    1) != 0;
    bFoggy      = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Foggy",      1) != 0;
    bLightning  = iniReader.ReadBoolean("VOLUMETRICWEATHERS", "Lightning",  1) != 0;

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

    // [VEHICLELIGHTS]
    bDualVehicleLights = iniReader.ReadInteger("VEHICLELIGHTS", "DualVehicleLights", 0) != 0;

    fHeadlightsCoronaSize      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "HeadlightsCoronaSize",      0.25f), 0.0f, 1.0f);
    fHeadlightsCoronaIntensity = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "HeadlightsCoronaIntensity", 0.1f),  0.0f, 1.0f);
    fTaillightsCoronaSize      = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "TaillightsCoronaSize",      0.25f), 0.0f, 1.0f);
    fTaillightsCoronaIntensity = std::clamp(iniReader.ReadFloat("VEHICLELIGHTS", "TaillightsCoronaIntensity", 0.1f),  0.0f, 1.0f);

    // [PICKUPLIGHTS]
    iPickupLightsMode = iniReader.ReadInteger("PICKUPLIGHTS", "PickupLightsMode", 1);
}

bool HasVolumes(CWeather::eWeatherType type)
{
    switch (type)
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

float SpotLightVolumeIntensities(CWeather::eWeatherType type)
{
    switch (type)
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

float PointLightVolumeIntensities(CWeather::eWeatherType type)
{
    switch (type)
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

float SpotLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
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

float PointLightVolumeScales(CWeather::eWeatherType type)
{
    switch (type)
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

// FusionFix code, slightly modified to get rid of the events' stuff
void OnAfterCopyLight(rage::CLightSource*);
static inline SafetyHookInline shCopyLight{};

static rage::CLightSource* __fastcall CopyLight(void* _this, void* edx, void* a2)
{
    auto ret = shCopyLight.fastcall<rage::CLightSource*>(_this, edx, a2);
    OnAfterCopyLight(ret);

    return ret;
}

void OnAfterCopyLight(rage::CLightSource *light)
{
    //#define RGB(R, G, B) { R / 255.0f, G / 255.0f, B / 255.0f };
    CWeather::eWeatherType CurrentWeather = CWeather::GetOldWeatherType();
    CWeather::eWeatherType NextWeather  = CWeather::GetNewWeatherType();
    float InterpolationValue = CWeather::GetWeatherInterpolationValue();

    if ((HasVolumes(CurrentWeather) || HasVolumes(NextWeather)) && !IsFusionFixSnowEnabled())
    {
        // Include spotlights and only those "flagged" with "LuminescenceHash" 57005
        if (light->mType == rage::LT_SPOT && light->mProjTexHash == 0xDEAD)
        {
            // Append the light shaft flag
            light->mFlags |= 8;

            // Transition from no volumes to volumes
            if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * SpotLightVolumeIntensities(NextWeather) * InterpolationValue;
                light->mVolumeScale = SpotLightVolumeScales(NextWeather) * InterpolationValue;
            }
            // Transition between volumes
            else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                float CurrentVolumeIntensity = SpotLightVolumeIntensities(CurrentWeather);
                float NextVolumeIntensity = SpotLightVolumeIntensities(NextWeather);

                float CurrentVolumeScale = SpotLightVolumeScales(CurrentWeather);
                float NextVolumeScale = SpotLightVolumeScales(NextWeather);

                light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue);
                light->mVolumeScale = CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue;
            }
            // Transition from volumes to no volumes
            else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * SpotLightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue);
                light->mVolumeScale = SpotLightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
            }
        }

        // Include pointlights and only those "flagged" with "LuminescenceHash" 57005
        if (light->mType == rage::LT_POINT && light->mProjTexHash == 0xDEAD)
        {
            // Append the light shaft flag
            light->mFlags |= 8;

            // Transition from no volumes to volumes
            if (!HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * PointLightVolumeIntensities(NextWeather) * InterpolationValue;
                light->mVolumeScale = PointLightVolumeScales(NextWeather) * InterpolationValue;
            }
            // Transition between volumes
            else if (HasVolumes(CurrentWeather) && HasVolumes(NextWeather))
            {
                float CurrentVolumeIntensity = PointLightVolumeIntensities(CurrentWeather);
                float NextVolumeIntensity = PointLightVolumeIntensities(NextWeather);

                float CurrentVolumeScale = PointLightVolumeScales(CurrentWeather);
                float NextVolumeScale = PointLightVolumeScales(NextWeather);

                light->mVolumeIntensity = 4.0f * (CurrentVolumeIntensity + (NextVolumeIntensity - CurrentVolumeIntensity) * InterpolationValue);
                light->mVolumeScale = CurrentVolumeScale + (NextVolumeScale - CurrentVolumeScale) * InterpolationValue;
            }
            // Transition from volumes to no volumes
            else if (HasVolumes(CurrentWeather) && !HasVolumes(NextWeather))
            {
                light->mVolumeIntensity = 4.0f * PointLightVolumeIntensities(CurrentWeather) * (1.0f - InterpolationValue);
                light->mVolumeScale = PointLightVolumeScales(CurrentWeather) * (1.0f - InterpolationValue);
            }
        }
    }

    // TODO: Rotterdam Tower custom light colors
    // This idea sadly might end up being scrapped, due to there being only one real approach of doing this, which kind of pushes the game's limits.
    // It consists of checking for custom projtex hashes for every light on each row of neons on the tower respectively.
    // The way the lights are done on the tower is: LOD1 - Lights / LOD2 - Emissive / LOD3 - Emissive.
    // To make this work we have to null out the emissive intensity from the low LODs and patch the high LODs' lights with the check, then copy the lights to the low LOD models as is as well.
    // While it just works, lights are really limited in this game and by doing this it means we're drawing over 50 lights at ALL times instead of just when near the tower.
    // That means from the start, out of 640 possible lights while away from the tower, we remain with around 590 that could be used by other lights, which is okay but if one were to increase the lamppost lights' distance
    // (!!!Including by raising the distance sliders especially!!!), the tower would have higher and higher chances of getting visible seizures within its lights.

    /*auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto date = std::localtime(&now_c);

    // Bottom Rotterdam Tower lights
    if (light->mProjTexHash == 0x0BBE)
    {
        if (IsFusionFixSnowEnabled())
        {
            light->mColor = RGB(255, 0, 0);
        }

        if (IsFusionFixHalloweenEnabled())
        {
            light->mColor = RGB(210, 128, 60);
        }

        // 29th April
        if ((date->tm_mon == 3 && date->tm_mday <= 29))
        {
            light->mColor = RGB(160, 32, 240);
        }
    }

    // Middle Rotterdam lights
    if (light->mProjTexHash == 0xB16)
    {
        if (IsFusionFixSnowEnabled())
        {
            light->mColor = RGB(0, 255, 0);
        }

        if (IsFusionFixHalloweenEnabled())
        {
            light->mColor = RGB(255, 165, 100);
        }

        // 29th April
        if ((date->tm_mon == 3 && date->tm_mday <= 29))
        {
            light->mColor = RGB(173, 216, 230);
        }
    }

    // Top Rotterdam lights
    if (light->mProjTexHash == 0xDADD8)
    {
        if (IsFusionFixSnowEnabled())
        {
            light->mColor = RGB(0, 255, 0);
        }

        if (IsFusionFixHalloweenEnabled())
        {
            light->mColor = RGB(100, 255, 175);
        }

        // 29th April
        if ((date->tm_mon == 3 && date->tm_mday <= 29))
        {
            light->mColor = RGB(0, 255, 255);
        }
    }*/
}

static uintptr_t Resume1 = 0;
static uintptr_t Resume2 = 0;
float* dwInnerConeAngle = nullptr;
float* dwOuterConeAngle = nullptr;

typedef	void* (__cdecl* AddSingleVehicleLight_T)(rage::Matrix44* TransformationMatrix, float* Position, rage::Vector3* Direction, rage::Vector3* Color, float Intensity, float Radius, float InnerConeAngle, float OuterConeAngle, int InteriorIndex, int RoomIndex, int ShadowCacheIndex, char a12, char a13);
AddSingleVehicleLight_T AddSingleVehicleLight = nullptr;

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

        mov eax, Resume1
        add eax, 11
        jmp eax
    }
}

void __declspec(naked) RenderCenterTaillightStub()
{
    __asm
    {
        mov ecx, [esp + 0x58];
        mov eax, [esp + 0x50];

        push ecx;
        push eax;

        push dword ptr[ebp + 0x2C];

        call RenderCenterTaillight;
        add esp, 0x38;

        mov	eax, Resume2;
        add	eax, 16;
        jmp	eax;
    }
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        ReadIni();

        // CopyLight hook
        {
            // FusionFix code, slightly modified to add version detection
            auto pattern = find_pattern("E8 ? ? ? ? F3 0F 10 44 24 ? 51 F3 0F 11 04 24 56 E8 ? ? ? ? 83 C4 08 FF 05", "E8 ? ? ? ? D9 44 24 0C 51 D9 1C 24 56 E8 ? ? ? ? 83 C4 08");
            if (!pattern.empty())
            {
                shCopyLight = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).get<void*>(), CopyLight);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }
        }

        // Weather hooks
        {
            // FusionFix code, slightly modified to add version detection
            auto pattern = find_pattern("A1 ? ? ? ? 83 C4 08 8B CF", "A1 ? ? ? ? 80 3F 04");
            if (!pattern.empty())
            {
                CWeather::OldWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }

            // FusionFix code, slightly modified to add version detection
            pattern = find_pattern("A1 ? ? ? ? 89 46 4C A1", "A1 ? ? ? ? 77 05 A1 ? ? ? ? 80 3F 04");
            if (!pattern.empty())
            {
                CWeather::NewWeatherType = *pattern.get_first<CWeather::eWeatherType*>(1);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }

            // FusionFix code, slightly modified to add version detection
            pattern = hook::pattern("F3 0F 10 05 ? ? ? ? 8B 44 24 0C 8B 4C 24 04");
            if (!pattern.empty())
            {
                CWeather::InterpolationValue = *pattern.get_first<float*>(4);
            }
            else
            {
                DisplayUnsupportedError();

                return false;
            }
        }

        // ParticleAttrs limit hook, requires an increase so TBoGT doesn't poof with all the provided .ide files which add a bunch of particles (Uses FusionFix code)
        {
            auto pattern = hook::pattern("8B C8 E8 ? ? ? ? B9 ? ? ? ? A3");
            auto CModelInfoStore__ms_baseModels = *pattern.get_first<CModelInfoStore::CDataStore*>(8);

            // We check if the array size is vanilla and only then increase it by two, to ensure we don't interfere with other limit adjusters here
            if (CModelInfoStore__ms_baseModels[CModelInfoStore::ms_particleAttrs].nSize == 0x0A8C)
            {
                CModelInfoStore__ms_baseModels[CModelInfoStore::ms_particleAttrs].nSize *= 2;
            }
        }

        // TODO: Dual vehicle light hooks, thanks to @xoxor4d (https://github.com/xoxor4d)
        {
            if (bDualVehicleLights)
            {
                // Single light function
                auto pattern = hook::pattern("55 8B EC 83 E4 F0 83 EC 20 80 7D 34 00 8B 4D 08 8B 45 10");
                AddSingleVehicleLight = (AddSingleVehicleLight_T)pattern.get_first(0);

                // Headlights
                {
                    auto pattern = hook::pattern("FF 75 24 E8 ? ? ? ? 83 C4 30 5F 5E 8B E5 5D C2 24 00");
                    injector::MakeNOP(pattern.get_first(0), 8, true);
                    injector::MakeJMP(pattern.get_first(0), RenderCenterHeadlightStub, true);
                    uintptr_t BaseAddress1 = (uintptr_t)pattern.get_first(0);
                    Resume1 = BaseAddress1;

                    // Right light position override?
                    pattern = hook::pattern("F3 0F 11 44 24 ? E8 ? ? ? ? 8D 44 24 60 50");
                    injector::MakeNOP(pattern.get_first(0), 6, true);

                    // Right light position override read? No idea if needed or what it even does :)
                    pattern = hook::pattern("F3 0F 10 74 24 ? F3 0F 59 25 ? ? ? ? F3 0F 11 74 24 ? F3 0F 59 2D ? ? ? ? F3 0F 11 64 24 ? 6A 00");
                    injector::MakeNOP(pattern.get_first(0), 6, true);

                    // Cone angle dwords
                    pattern = hook::pattern("F3 0F 59 15 ? ? ? ? F3 0F C2 C3 06 F3 0F 10 1D ? ? ? ? F3 0F 59 1D ? ? ? ? F3 0F 11 54 24 ? 0F 54 C6");
                    dwInnerConeAngle = *pattern.get_first<float*>(4);
                    dwOuterConeAngle = *pattern.get_first<float*>(25);
                }

                // Taillights
                {
                    auto pattern = hook::pattern("8D 44 24 ? 50 FF 75 ? E8 ? ? ? ? 83 C4 ? 5F 5E 8B E5 5D C2 ? ? 83 FF ? 0F 84 ? ? ? ? 80 7D ? ? 0F 85 ? ? ? ? F3 0F 10 05");
                    injector::MakeJMP(pattern.get_first(0), RenderCenterTaillightStub, true);
                    uintptr_t BaseAddress2 = (uintptr_t)pattern.get_first(0);
                    Resume2 = BaseAddress2;
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
                    injector::WriteMemory(pattern.get_first(1), 0xD3, true); // For flags, push 201 --> push 211 (+Fill lighting)
                }
                else
                {
                    pattern = hook::pattern("68 ? ? ? ? 6A 00 6A 00 F3 0F 11 54 24 ? E8 ? ? ? ? 83 C6 01 83 C4 40 83 C7 10");
                    injector::WriteMemory(pattern.get_first(1), 0xD3, true); // For flags, push 201 --> push 211 (+Fill lighting)
                }
            }
        }
    }

    return true;
}
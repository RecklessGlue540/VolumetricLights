#include "Utility.h"

#include "rage/LightSource.h"
#include "rage/Matrix.h"
#include "rage/Vector.h"

void ReadIni()
{
    CIniReader iniReader("");

    // TODO
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
    }

    return true;
}
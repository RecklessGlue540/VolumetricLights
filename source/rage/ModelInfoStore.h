#pragma once

#include "../Utility.h"

// From https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/11b4eee7dad99fb21c251dae3b88439b1e073d45/source/limits.ixx#L384

namespace CModelInfoStore
{
    enum CModelInfoStore
    {
        ms_baseModels,
        ms_instanceModels,
        ms_timeModels,
        ms_weaponModels,
        ms_vehicleModels,
        ms_pedModels,
        ms_mloModels,
        ms_mlo,
        stru_F27FC4,
        ms_amat,
        ms_2dfxRefs1,
        ms_2dfxRefs2,
        ms_particleAttrs,
        ms_explosionAttrs,
        ms_procObjsAttrs,
        ms_ladderInfo,
        ms_spawnPointAttrs,
        ms_lightShaftAttrs,
        ms_scrollBars,
        ms_swayableAttrs,
        ms_bouyancyAttrs,
        ms_audioAttrs,
        ms_worldPointAttrs,
        ms_walkDontWalkAttrs,

        amount
    };

    struct CDataStore
    {
        uint32_t nSize;
        uint32_t nAllocated;
        uint32_t pData;
    };
}
# VolumetricLights

## Info
A mod for Grand Theft Auto IV which adds fully customizable volumetric shafts to select streetlights during appropriate weather conditions.

It also features numerous fixes and improvements to the affected streetlight models.

> [!NOTE]
>
> This is a continuation of the old mod by [AssaultKifle47](https://github.com/akifle47). Kudos go to him for the base of all these changes.
>

### Options/.ini sections rundown:

### Weather toggles

The options in the [VOLUMETRICWEATHERS] section allow choosing which weathers to enable the volumetric shafts for:

```ini
[VOLUMETRICWEATHERS]
ExtraSunny = 0
Sunny      = 0
SunnyWindy = 0
Cloudy     = 0
Rain       = 1
Drizzle    = 1
Foggy      = 1
Lightning  = 1
```

### Spotlight volumes

The options in the [SPOTLIGHTS] section allow customizing volume intensity and scale per-weather, for the affected spotlights:

```ini
[SPOTLIGHTS]
// Threshold [0.0 | 1.5]
SpotLightsVolumeIntensityExtraSunny = 0.0
SpotLightsVolumeIntensitySunny      = 0.0
SpotLightsVolumeIntensitySunnyWindy = 0.0
SpotLightsVolumeIntensityCloudy     = 0.0
SpotLightsVolumeIntensityRain       = 1.15
SpotLightsVolumeIntensityDrizzle    = 1.1
SpotLightsVolumeIntensityFoggy      = 1.3
SpotLightsVolumeIntensityLightning  = 1.2

// Threshold [0.0 | 0.5]
SpotLightsVolumeScaleExtraSunny = 0.0
SpotLightsVolumeScaleSunny      = 0.0
SpotLightsVolumeScaleSunnyWindy = 0.0
SpotLightsVolumeScaleCloudy     = 0.0
SpotLightsVolumeScaleRain       = 0.15
SpotLightsVolumeScaleDrizzle    = 0.1
SpotLightsVolumeScaleFoggy      = 0.3
SpotLightsVolumeScaleLightning  = 0.2
```

### Pointlight volumes

The options in the [POINTLIGHTS] section allow customizing volume intensity and scale per-weather, for the affected pointlights:

```ini
[POINTLIGHTS]
// Threshold [0.0 | 1.5]
PointLightsVolumeIntensityExtraSunny = 0.0
PointLightsVolumeIntensitySunny      = 0.0
PointLightsVolumeIntensitySunnyWindy = 0.0
PointLightsVolumeIntensityCloudy     = 0.0
PointLightsVolumeIntensityRain       = 1.1
PointLightsVolumeIntensityDrizzle    = 1.075
PointLightsVolumeIntensityFoggy      = 1.15
PointLightsVolumeIntensityLightning  = 1.125

// Threshold [0.0 | 0.5]
PointLightsVolumeScaleExtraSunny = 0.0
PointLightsVolumeScaleSunny      = 0.0
PointLightsVolumeScaleSunnyWindy = 0.0
PointLightsVolumeScaleCloudy     = 0.0
PointLightsVolumeScaleRain       = 0.1
PointLightsVolumeScaleDrizzle    = 0.075
PointLightsVolumeScaleFoggy      = 0.15
PointLightsVolumeScaleLightning  = 0.125
```

### Vehicle coronas

The options in the [VEHICLELIGHTS] section allow customizing corona size and intensity, for vehicle headlights and taillights:

```ini
[VEHICLELIGHTS]
// Threshold [0.0 | 1.0]
HeadlightsCoronaSize       = 0.25
HeadlightsCoronaIntensity  = 0.1
TaillightsCoronaSize       = 0.25
TaillightsCoronaIntensity  = 0.1
```

Normally, casual players do not need to edit these options unless they're not enough as is.\
They are provided more for timecycle modders who will definitely need them.

## Compatibility

Supported game versions: 1.0.7.0 up to 1.2.0.59. Any other versions are not and are not going to be supported.

## Requirements

[FusionFix](https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix) is required for the installation and for all the fixes/improvements it brings to artificial lights and volumetric shafts.

> [!IMPORTANT]
>
> The provided .img archive with the models in this mod is required for the volumetric shafts to work.
>
> Other mods which change the same light models as this mod would have to be made compatible.\
> FusionFix is excluded, as all the changes of its light models are pretty much already present here as well.
>

## Installation

Download the latest release, open the .7z archive and copy the "plugins" and "update" folders from it over to your game's directory.

> [!NOTE]
>
> The mod comes with a high update folder priority already, so to avoid conflicts with other light mods.\
> Note that it should always come before FusionFix's folder, otherwise none of the mod will work.
>

## Limitations

- Distant fading for the volumes of the affected lights does not work correctly, likely due to the way the volumes are applied, which may not be fixable.

## Asset Changelog

TODO: Complete this

### brownstones.img
- **shitkitch/dodgyraylight.wdr**
  - Moved the light source from shitkitch.wdr to dodgyraylight.wdr to fix its position;
  - Included the [volume properties from the console versions](https://github.com/Parallellines0451/GTAIV.EFLC.FusionShaders/blob/2d68add8f58f97e9851c59432146f3b188194646/assets/references/console_light_flags/brownstones.img/shitkitch/shitkitch.light#L10) of the game;
  - Adjusted the bulb's vertex color to that of the light's color;
  - Adjusted the bulb's emissive intensity to that of the light's intensity;

### lamppost.ide

- Draw distances of lamppost models have been made consistent (e.g. All lampposts have a distance of 150 now, while wall lights have a distance of 100);
- Fixed and improved particle effects (Primarily when shooting bulbs);

### lamppost.img

- Light source positions relative to the bulbs of the models have been improved, making the volumetric shafts and/or shadows make sense;
- All the bulbs of the lamppost models had their vertex colors adjusted to match their actual light colors;
- All lamppost models can no longer be occluded in night shadows, resolving visual weirdness when they have shadows enabled;
- Corona sizes have been lowered overall, while intensities have been made more consistent;
- Fixed materials (e.g. Bulbs now use a glass shader that allows the cracks texture to show when shot, where applicable);
- Made shadow flags consistent: All lamppost models can now cast shadows including both static geometry and dynamic objects, if allowed;

### traffic.ide
- Draw distances of traffic light models have been made consistent (e.g. All traffic lights have a distance of 150 now, while traffic lights without the RGY light have a distance of 100);
- Fixed and improved particle effects (Primarily when shooting bulbs);

### traffic.img

- Adjusted traffic light circle positions, fixing clipping;
- Nulled hidden leftover/test(?) light sources from traffic lights, that might have had a significant influence on the game's 640 light maps limit;
- All traffic light models can no longer be occluded in night shadows, resolving visual weirdness when shadows from other lights occlude them.

### To-do:

High priority:
- Add a Project2DFX config with adjusted light positions;
- Improve destructibility of certain lamppost parts if viable (Such as traffic light boxes, bm_nylamp7 lamps, etc);
- Fix bm_wall_light_02 emissive glass z-shifting;
- Fix collisions of some wall light models to allow the cracks texture to show on their glass materials when shot;

Mid priority:
- Fix/Improve more models;

Lower priority:
- Adjust WalkDontWalk signal texture offsets;
- Investigate bm_nytraflite2b and bm_nytrafflite_11 yellow traffic light circle bug;
- Make bm_nylamp1c and bm_nylamp6c ped signal boxes [functional](https://github.com/valentyn-l/GTAIV.EFLC.Various.Fixes#pedestrian-traffic-lights) without bugs [#1](https://tsmsportz.com/postimg/14VZRdqp), if even at all possible.

## Credits:

- [UnknownLITE](https://github.com/UnknownLITE), for his Blender Add-On that helped make all the models' work possible;

- [FusionFix](https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix):
  - For the more straightforward and compatible hooks ([#1](https://github.com/RecklessGlue540/VolumetricLights/blob/dcd210c1a5aa9ca4207de41d0edac5f2ef9f092b/source/Main.cpp#L145), [#2](https://github.com/RecklessGlue540/VolumetricLights/blob/dcd210c1a5aa9ca4207de41d0edac5f2ef9f092b/source/Main.cpp#L242), [#3](https://github.com/RecklessGlue540/VolumetricLights/blob/dcd210c1a5aa9ca4207de41d0edac5f2ef9f092b/source/Utility.h#L12), [#4](https://github.com/RecklessGlue540/VolumetricLights/blob/dcd210c1a5aa9ca4207de41d0edac5f2ef9f092b/source/rage/Weather.h#L25)) - All of this code is licensed under GPL v3, rest is MIT licensed, as the original mod;
  - For the emissive materials trick which allows disabling night shadows on certain objects ([#1](https://gtaforums.com/topic/934545-fusion-fix/?do=findComment&comment=1072386201), [#2](https://gtaforums.com/topic/934545-fusion-fix/?do=findComment&comment=1072386316)).

## Reporting issues

Encountered a problem caused by this mod? Feel free to [open an issue](https://github.com/RecklessGlue540/VolumetricLights/issues).

Encountered a light model in the game that could use touch-ups or volumetric shafts?\
Feel free to [open an issue](https://github.com/RecklessGlue540/VolumetricLights/issues) showing it + the location it's at in the game world.

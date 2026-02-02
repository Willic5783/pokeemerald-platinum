#include "gba_dual_slot.h"
#include "battle.h"
#include "constants/region_map_sections.h"
#include "data.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_message_box.h"
#include "fieldmap.h"
#include "global.h"
#include "international_string_util.h"
#include "overworld.h"
#include "pokedex.h"
#include "region_map.h"
#include "rtc.h"
#include "script_menu.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"

// Function declarations
static bool8 BernoulliTrial(u16);
static void TVShowDone(void);
void StartGBACartEncounters(void);

// Define statements
#define LAST_TVSHOW_IDX (TV_SHOWS_COUNT - 1)
#define rbernoulli(num, den) BernoulliTrial(0xFFFF * (num) / (den))

static EWRAM_DATA ALIGNED(4) u8 sTVShowState = 0;

static bool8 BernoulliTrial(u16 ratio) {
  if (Random() <= ratio)
    return FALSE;

  return TRUE;
}

u8 *const gGBAStringVarPtrs[] = {gStringVar1, gStringVar2, gStringVar3};


static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
} sPokeOutbreakSpeciesList[] = {
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_BIDE, MOVE_HARDEN, MOVE_LEECH_SEED},
        .level = 3,
        .location = MAP_NUM(MAP_ROUTE102)
    }
};

static const u8 *const sTVMassOutbreakTextGroup[] = {gTVMassOutbreakText00};

static void DoTVShowPokemonNewsMassOutbreak(void) {
      switch (VarGet(VAR_GBA_CART)) {
  case VAR_GBA_CART_FR:
    break;
  case VAR_GBA_CART_LG:
    break;
  case VAR_GBA_CART_R:
    break;
  case VAR_GBA_CART_S:
    break;
  case VAR_GBA_CART_E:
    break;
  default:
    break;
  }

  TVShow *show;

  show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
  GetMapName(gStringVar1, show->massOutbreak.locationMapNum, 0);
  StringCopy(gStringVar2, GetSpeciesName(show->massOutbreak.species));
  TVShowDone();
  StartGBACartEncounters();
  ShowFieldMessage(sTVMassOutbreakTextGroup[sTVShowState]);
}

void TryStartGBAEncounters(void) {
  u8 i;
  u16 outbreakIdx;
  TVShow *show;

  outbreakIdx = 0;
  show->massOutbreak.kind = TVSHOW_MASS_OUTBREAK;
  show->massOutbreak.active = TRUE;
  show->massOutbreak.level = sPokeOutbreakSpeciesList[outbreakIdx].level;
  show->massOutbreak.unused1 = 0;
  show->massOutbreak.unused3 = 0;
  show->massOutbreak.species = sPokeOutbreakSpeciesList[outbreakIdx].species;
  show->massOutbreak.unused2 = 0;
  show->massOutbreak.moves[0] = sPokeOutbreakSpeciesList[outbreakIdx].moves[0];
  show->massOutbreak.moves[1] = sPokeOutbreakSpeciesList[outbreakIdx].moves[1];
  show->massOutbreak.moves[2] = sPokeOutbreakSpeciesList[outbreakIdx].moves[2];
  show->massOutbreak.moves[3] = sPokeOutbreakSpeciesList[outbreakIdx].moves[3];
  show->massOutbreak.locationMapNum =
      sPokeOutbreakSpeciesList[outbreakIdx].location;
  show->massOutbreak.locationMapGroup = 0;
  show->massOutbreak.unused4 = 0;
  show->massOutbreak.probability = 100;
  show->massOutbreak.unused5 = 0;
  show->massOutbreak.daysLeft = 1;
  show->massOutbreak.language = gGameLanguage;

  for (i = 0; i < LAST_TVSHOW_IDX; i++) {
    if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_MASS_OUTBREAK)
      return;
  }
  if (!rbernoulli(1, 2)) {
  }
}

// THIS DOES NOT WORK CURRENTLY... FIX EVENTUALLY... TryStartGBAEncounters tries
// to start but can't??? src/wild_encounter.c -> static bool8
// SetUpMassOutbreakEncounter(u8 flags) && static bool8
// DoMassOutbreakEncounterTest(void) Copy this function and modify it to work
// with GBA Encounters ???
void StartGBACartEncounters(void) {
  TVShow *show = &gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004];
  gSaveBlock1Ptr->outbreakPokemonSpecies = show->massOutbreak.species;
  gSaveBlock1Ptr->outbreakLocationMapNum = show->massOutbreak.locationMapNum;
  gSaveBlock1Ptr->outbreakLocationMapGroup =
      show->massOutbreak.locationMapGroup;
  gSaveBlock1Ptr->outbreakPokemonLevel = show->massOutbreak.level;
  gSaveBlock1Ptr->outbreakUnused1 = show->massOutbreak.unused1;
  gSaveBlock1Ptr->outbreakUnused2 = show->massOutbreak.unused2;
  gSaveBlock1Ptr->outbreakPokemonMoves[0] = show->massOutbreak.moves[0];
  gSaveBlock1Ptr->outbreakPokemonMoves[1] = show->massOutbreak.moves[1];
  gSaveBlock1Ptr->outbreakPokemonMoves[2] = show->massOutbreak.moves[2];
  gSaveBlock1Ptr->outbreakPokemonMoves[3] = show->massOutbreak.moves[3];
  gSaveBlock1Ptr->outbreakUnused3 = show->massOutbreak.unused3;
  gSaveBlock1Ptr->outbreakPokemonProbability = show->massOutbreak.probability;
  gSaveBlock1Ptr->outbreakDaysLeft = 255;
}

// Goal is that when var 0x404E == 5, this void/script/thing
void EndGBACartEncounters(void) {
  gSaveBlock1Ptr->outbreakPokemonSpecies = SPECIES_NONE;
  gSaveBlock1Ptr->outbreakLocationMapNum = 0;
  gSaveBlock1Ptr->outbreakLocationMapGroup = 0;
  gSaveBlock1Ptr->outbreakPokemonLevel = 0;
  gSaveBlock1Ptr->outbreakUnused1 = 0;
  gSaveBlock1Ptr->outbreakUnused2 = 0;
  gSaveBlock1Ptr->outbreakPokemonMoves[0] = MOVE_NONE;
  gSaveBlock1Ptr->outbreakPokemonMoves[1] = MOVE_NONE;
  gSaveBlock1Ptr->outbreakPokemonMoves[2] = MOVE_NONE;
  gSaveBlock1Ptr->outbreakPokemonMoves[3] = MOVE_NONE;
  gSaveBlock1Ptr->outbreakUnused3 = 0;
  gSaveBlock1Ptr->outbreakPokemonProbability = 0;
  gSaveBlock1Ptr->outbreakDaysLeft = 0;
}

static void TVShowDone(void) {
  gSpecialVar_Result = TRUE;
  sTVShowState = 0;
  gSaveBlock1Ptr->tvShows[gSpecialVar_0x8004].common.active = FALSE;
}

// Unused???
static void TryEndMassOutbreak(u16 days) {
  if (gSaveBlock1Ptr->outbreakDaysLeft <= days)
    EndGBACartEncounters();
  else
    gSaveBlock1Ptr->outbreakDaysLeft -= days;
}

// Unused???
// static void UpdateMassOutbreakTimeLeft(u16 days)
// {
//     u8 i;
//     TVShow *show;

//     if (gSaveBlock1Ptr->outbreakPokemonSpecies == SPECIES_NONE)
//     {
//         for (i = 0; i < LAST_TVSHOW_IDX; i++)
//         {
//             if (gSaveBlock1Ptr->tvShows[i].massOutbreak.kind ==
//             TVSHOW_MASS_OUTBREAK &&
//             gSaveBlock1Ptr->tvShows[i].massOutbreak.active == TRUE)
//             {
//                 show = &gSaveBlock1Ptr->tvShows[i];
//                 if (show->massOutbreak.daysLeft < days)
//                     show->massOutbreak.daysLeft = 0;
//                 else
//                     show->massOutbreak.daysLeft -= days;

//                 break;
//             }
//         }
//     }
// }

// enum TimeOfDay CheckGBACartForEncounters(u32 headerId, enum WildPokemonArea
// area)
// {
//     const struct WildPokemonInfo *wildMonInfo;
//     enum TimeOfDay timeOfDay = GetTimeOfDay();

//     if (!OW_TIME_OF_DAY_ENCOUNTERS)
//         return TIME_OF_DAY_DEFAULT;

//     // if (InBattlePike() || CurrentBattlePyramidLocation() !=
//     PYRAMID_LOCATION_NONE)
//         // return OW_TIME_OF_DAY_FALLBACK;

//     switch (area)
//     {
//     default:
//     case WILD_AREA_LAND:
//         wildMonInfo =
//         gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo;
//         break;
//     case WILD_AREA_WATER:
//         wildMonInfo =
//         gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;
//         break;
//     case WILD_AREA_ROCKS:
//         wildMonInfo =
//         gWildMonHeaders[headerId].encounterTypes[timeOfDay].rockSmashMonsInfo;
//         break;
//     case WILD_AREA_FISHING:
//         wildMonInfo =
//         gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo;
//         break;
//     case WILD_AREA_HIDDEN:
//         wildMonInfo =
//         gWildMonHeaders[headerId].encounterTypes[timeOfDay].hiddenMonsInfo;
//         break;
//     }

//     if (wildMonInfo == NULL && !OW_TIME_OF_DAY_DISABLE_FALLBACK)
//         return OW_TIME_OF_DAY_FALLBACK;
//     else
//         return GenConfigTimeOfDay(timeOfDay);
// }

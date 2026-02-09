#include "global.h"
#include "gba_dual_slot.h"
#include "battle.h"
#include "event_data.h"
#include "event_scripts.h"
#include "field_message_box.h"
#include "fieldmap.h"
#include "region_map.h"
#include "string_util.h"

// Function declarations
static void TVShowDone(void);
void StartGBACartEncounters(void);
static void SwitchGBACart (void);


// Define statements
#define LAST_TVSHOW_IDX (TV_SHOWS_COUNT - 1)
#define rbernoulli(num, den) BernoulliTrial(0xFFFF * (num) / (den))

static EWRAM_DATA ALIGNED(4) u8 sTVShowState = 0;

u8 *const gGBAStringVarPtrs[] = {gStringVar1, gStringVar2, gStringVar3};

static const u8 *const sTVMassOutbreakTextGroup[] = {gTVMassOutbreakText00};

static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
}
// Fire Red
sPokeOutbreakSpeciesList[] = {
    {
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_BITE, MOVE_ROAR},
        .level = 2,
        .location = MAP_NUM(MAP_ROUTE201)
    },
    {
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_BITE, MOVE_ROAR},
        .level = 2,
        .location = MAP_NUM(MAP_ROUTE202)
    },
    {
        .species = SPECIES_CATERPIE,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204_NORTH)
    },
    {
        .species = SPECIES_CATERPIE,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204)
    }
    // },
    // {
    //     .species = SPECIES_EKANS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE212SOUTH)
    // },
    // {
    //     .species = SPECIES_PELIPPER,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE230)
    // },
    // {
    //     .species = SPECIES_CATERPIE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ETERNA_FOREST)
    // },
    // {
    //     .species = SPECIES_ARBOK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_GREAT_MARSH_1)
    // },
    // {
    //     .species = SPECIES_ARBOK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_GREAT_MARSH_2)
    // },
    // {
    //     .species = SPECIES_ARBOK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_GREAT_MARSH_3)
    // },
    // {
    //     .species = SPECIES_ARBOK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_GREAT_MARSH_4)
    // },
    // {
    //     .species = SPECIES_ARBOK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_GREAT_MARSH_5)
    // },
    // {
    //     .species = SPECIES_ARBOK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_GREAT_MARSH_6)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_4F)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_5F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_1F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_B1F)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_1)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_2)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_3)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_4)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_5)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_6)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_1)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_4)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_5)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_6)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_7)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_8)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_9)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_WAYBACK_CAVE)
    // }
};


static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
}
sPokeOutbreakSpeciesListLG[] = {
    {
        .species = SPECIES_WEEDLE,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204_NORTH)
    },
    {
        .species = SPECIES_WEEDLE,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204)
    }
    // }, comment this back in later, mut have comma for full list
    // {
    //     .species = SPECIES_VULPIX,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE209)
    // },
    // {
    //     .species = SPECIES_VULPIX,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE214)
    // },
    // {
    //     .species = SPECIES_SANDSLASH,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE228)
    // },
    // {
    //     .species = SPECIES_PELIPPER,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE230)
    // },
    // {
    //     .species = SPECIES_WEEDLE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ETERNA_FOREST)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_4F)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_5F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_1F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_B1F)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_1)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_2)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_3)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_4)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_5)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_6)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_1)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_4)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_5)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_6)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_7)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_8)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_9)
    // },
    // {
    //     .species = SPECIES_SANDSHREW,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_WAYBACK_CAVE)
    // },
    // {
    //     .species = SPECIES_SANDSHREW,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_WAYBACK_CAVE_SECRET)
    // }
};


static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;

}
sPokeOutbreakSpeciesListR[] = {
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_BIDE, MOVE_HARDEN},
        .level = 5,
        .location = MAP_NUM(MAP_ROUTE203)
    },
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204_NORTH)
    },
    {
        .species = SPECIES_SEEDOT,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204)
    }
    // },
    // {
    //     .species = SPECIES_ZANGOOSE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE208)
    // },
    // {
    //     .species = SPECIES_ZANGOOSE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE210NORTH)
    // },
    // {
    //     .species = SPECIES_ZANGOOSE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE210SOUTH)
    // },
    // {
    //     .species = SPECIES_NUZLEAF,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE229)
    // },
    // {
    //     .species = SPECIES_PELIPPER,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE230)
    // },
    // {
    //     .species = SPECIES_SEEDOT,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ETERNA_FOREST)
    // },
    // {
    //     .species = SPECIES_MAWILE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B1F_LEFT)
    // },
    // {
    //     .species = SPECIES_MAWILE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B1F_RIGHT)
    // },
    // {
    //     .species = SPECIES_MAWILE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B2F_LEFT)
    // },
    // {
    //     .species = SPECIES_MAWILE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B2F_RIGHT)
    // },
    // {
    //     .species = SPECIES_MAWILE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B3F)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_ACUITY)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_VALOR)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_VALOR)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_4F)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_5F)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_2F)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_2F_BACK)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_3F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_3F_SUMMIT)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_4F_ABOVE)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_4F_WATERFALL)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_5F)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_6F)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_TUNNEL)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_1F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_B1F)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_SENDOFF_SPRING)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_1)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_2)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_3)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_4)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_5)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_6)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_1)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_10)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_11)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_12)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_13)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_14)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_15)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_16)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_17)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_18)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_19)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_2)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_20)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_21)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_22)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_23)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_24)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_25)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_26)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_27)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_28)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_29)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_3)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_30)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_31)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_32)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_33)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_34)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_4)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_5)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_6)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_8)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_9)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_1)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_2)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_3)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_4)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_5)
    // },
    // {
    //     .species = SPECIES_SOLROCK,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_6)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_WAYBACK_CAVE)
    // }
};


static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
}
sPokeOutbreakSpeciesListS[] = {
    {
        .species = SPECIES_LOTAD,
        .moves = {MOVE_ASTONISH, MOVE_GROWL, MOVE_ABSORB},
        .level = 5,
        .location = MAP_NUM(MAP_ROUTE203)
    },
    {
        .species = SPECIES_LOTAD,
        .moves = {MOVE_ASTONISH, MOVE_GROWL, MOVE_ABSORB},
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204_NORTH)
    },
    {
        .species = SPECIES_LOTAD,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204)
    }
    // },
    // {
    //     .species = SPECIES_LOTAD,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE205PREETERNA) // FIX
    // },
    // {
    //     .species = SPECIES_SEVIPER,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE208)
    // },
    // {
    //     .species = SPECIES_LOMBRE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE212SOUTH)
    // },
    // {
    //     .species = SPECIES_LOMBRE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE229)
    // },
    // {
    //     .species = SPECIES_PELIPPER,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE230)
    // },
    // {
    //     .species = SPECIES_SABLEYE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B1F_LEFT)
    // },
    // {
    //     .species = SPECIES_SABLEYE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B1F_RIGHT)
    // },
    // {
    //     .species = SPECIES_SABLEYE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B2F_LEFT)
    // },
    // {
    //     .species = SPECIES_SABLEYE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B2F_RIGHT)
    // },
    // {
    //     .species = SPECIES_SABLEYE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_IRON_ISLAND_B3F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_ACUITY)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_VALOR)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_VERITY)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_4F)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_5F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_2F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_2F_BACK)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_3F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_3F_SUMMIT)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_4F_ABOVE)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_4F_WATERFALL)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_5F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_6F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_MT_CORONET_TUNNEL)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_1F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_B1F)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_SENDOFF_SPRING)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_1)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_2)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_3)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_4)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_5)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_6)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_1)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_10)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_11)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_12)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_13)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_14)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_15)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_16)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_17)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_18)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_19)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_2)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_20)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_21)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_22)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_23)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_24)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_25)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_26)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_27)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_28)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_29)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_3)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_30)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_31)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_32)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_33)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_34)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_4)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_5)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_6)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_8)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_9)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_1)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_2)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_3)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_4)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_5)
    // },
    // {
    //     .species = SPECIES_LUNATONE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_3_6)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_WAYBACK_CAVE)
    // }
};


static const struct {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 location;
}
sPokeOutbreakSpeciesListE[] = {
    {
        .species = SPECIES_PINECO,
        .moves = {MOVE_TACKLE, MOVE_PROTECT},
        .level = 5,
        .location = MAP_NUM(MAP_ROUTE203)
    },
    {
        .species = SPECIES_PINECO,
        .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
        .level = 2, // FIX
        .location = MAP_NUM(MAP_ROUTE204_NORTH)
    }
    // },
    // {
    //     .species = SPECIES_PINECO,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_Route204)
    // },
    // {
    //     .species = SPECIES_PINECO,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE210NORTH)
    // },
    // {
    //     .species = SPECIES_PINECO,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE210SOUTH)
    // },
    // {
    //     .species = SPECIES_TEDDIURSA,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE211WEST)
    // },
    // {
    //     .species = SPECIES_URSARING,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE217)
    // },
    // {
    //     .species = SPECIES_SHUCKLE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE224)
    // },
    // {
    //     .species = SPECIES_PINECO,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE229)
    // },
    // {
    //     .species = SPECIES_PELIPPER,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ROUTE230)
    // },
    // {
    //     .species = SPECIES_URSARING,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKEFRONT_ACUITY)
    // },
    // {
    //     .species = SPECIES_PINECO,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_ETERNA_FOREST)
    // },
    // {
    //     .species = SPECIES_TEDDIURSA,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LAKE_ACUITY)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_4F)
    // },
    // {
    //     .species = SPECIES_GASTLY,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_LOST_TOWER_5F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_1F)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_OREBURGH_MINE_B1F)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_1)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_2)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_3)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_4)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_5)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_1_6)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_1)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_4)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_5)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_6)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_7)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_8)
    // },
    // {
    //     .species = SPECIES_DUSCLOPS,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_TURNBACK_CAVE_PILLAR_2_9)
    // },
    // {
    //     .species = SPECIES_GEODUDE,
    //     .moves = {MOVE_BITE, MOVE_ROAR}, // FIX
    //     .level = 2, // FIX
    //     .location = MAP_NUM(MAP_WAYBACK_CAVE)
    // }
};


static void SwitchGBACart (void) {
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

    if (FlagGet(FLAG_SYS_GAME_CLEAR)) {
        for (i = 0; i < LAST_TVSHOW_IDX; i++) {
            if (gSaveBlock1Ptr->tvShows[i].common.kind == TVSHOW_MASS_OUTBREAK)
                return;
        }
        outbreakIdx = 0;
        show = &gSaveBlock1Ptr->tvShows[0];
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
        show->massOutbreak.locationMapNum = sPokeOutbreakSpeciesList[outbreakIdx].location;
        show->massOutbreak.locationMapGroup = 0;
        show->massOutbreak.unused4 = 0;
        show->massOutbreak.probability = 100;
        show->massOutbreak.unused5 = 0;
        show->massOutbreak.daysLeft = 1;
        show->massOutbreak.language = gGameLanguage;
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






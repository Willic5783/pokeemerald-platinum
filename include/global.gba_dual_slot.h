#ifndef GUARD_GLOBAL_GBA_DUAL_SLOT_H
#define GUARD_GLOBAL_GBA_DUAL_SLOT_H

#include "constants/tv.h"
#include "constants/gba_dual_slot.h"


typedef union // size = 0x24
{
    // Common
    struct {
        /*0x00*/ u8 kind;
        /*0x01*/ bool8 active;
        /*0x02*/ u8 data[26];
        /*0x1C*/ u8 srcTrainerId3Lo;
        /*0x1D*/ u8 srcTrainerId3Hi;
        /*0x1E*/ u8 srcTrainerId2Lo;
        /*0x1F*/ u8 srcTrainerId2Hi;
        /*0x20*/ u8 srcTrainerIdLo;
        /*0x21*/ u8 srcTrainerIdHi;
        /*0x22*/ u8 trainerIdLo;
        /*0x23*/ u8 trainerIdHi;
    } common;

    // Common init (used for initialization loop)
    struct {
        /*0x00*/ u8 kind;
        /*0x01*/ bool8 active;
        /*0x02*/ u8 data[34];
    } commonInit;


    // TVSHOW_DUMMY
    struct {
        /*0x00*/ u8 kind;
        /*0x01*/ bool8 active;
        /*0x02*/ u16 words[2];
        /*0x06*/ u16 species;
        /*0x08*/ u8 filler_08[3];
        /*0x0B*/ u8 name[12];
        /*0x17*/ u8 language;
    } dummy;

    // Mass Outbreak
    // TVSHOW_MASS_OUTBREAK
    struct {
        /*0x00*/ u8 kind;
        /*0x01*/ bool8 active;
        /*0x02*/ u8 unused1;
        /*0x03*/ u8 unused3;
        /*0x04*/ u16 moves[MAX_MON_MOVES];
        /*0x0C*/ u16 species;
        /*0x0E*/ u16 unused2;
        /*0x10*/ u8 locationMapNum;
        /*0x11*/ u8 locationMapGroup;
        /*0x12*/ u8 unused4;
        /*0x13*/ u8 probability;
        /*0x14*/ u8 level;
        /*0x15*/ u8 unused5;
        /*0x16*/ u16 daysLeft;
        /*0x18*/ u8 language;
        /*0x19*/ //u8 padding;
    } massOutbreak;
} GameCart;




#endif //GUARD_GLOBAL_GBA_DUAL_SLOT_H
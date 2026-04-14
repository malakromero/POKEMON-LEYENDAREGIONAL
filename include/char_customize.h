#ifndef GUARD_CHAR_CUSTOMIZE_H
#define GUARD_CHAR_CUSTOMIZE_H

#include "global.h"

// Presets de color de piel (índices de paleta)
#define SKIN_COLOR_LIGHT    0
#define SKIN_COLOR_MEDIUM   1
#define SKIN_COLOR_DARK     2
#define SKIN_COLOR_COUNT    3

// Presets de color de ropa (índices de paleta)
#define OUTFIT_COLOR_DEFAULT  0
#define OUTFIT_COLOR_RED      1
#define OUTFIT_COLOR_BLUE     2
#define OUTFIT_COLOR_GREEN    3
#define OUTFIT_COLOR_YELLOW   4
#define OUTFIT_COLOR_COUNT    5

// Sprites de personaje disponibles (índices)
#define CHAR_SPRITE_BRENDAN  0
#define CHAR_SPRITE_MAY      1
#define CHAR_SPRITE_RED      2
#define CHAR_SPRITE_LEAF     3
#define CHAR_SPRITE_COUNT    4

// Estructura con la personalización elegida por el jugador
struct CharCustomization
{
    u8 spriteIndex;    // Índice de sprite de personaje (CHAR_SPRITE_*)
    u8 skinColor;      // Color de piel (SKIN_COLOR_*)
    u8 outfitColor;    // Color de ropa (OUTFIT_COLOR_*)
    u8 gender;         // MALE or FEMALE
};

extern struct CharCustomization gCharCustomization;
extern bool8 gSkipBirchPlayerSetup;

void CB2_InitCharCustomize(void);

#endif // GUARD_CHAR_CUSTOMIZE_H

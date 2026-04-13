#include "global.h"

const u32 gKantoRegionMapTiles[] = {
    #include "region_map.4bpp.inc"
};

const u16 gKantoRegionMapPalette[] = {
    #include "region_map.pal.inc"
};

const u32 gKantoRegionMapTilemap[] = {
    #include "region_map.bin.inc"
};

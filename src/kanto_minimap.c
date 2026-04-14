#include "global.h"
#include "bg.h"
#include "decompress.h"
#include "palette.h"
#include "window.h"
#include "region_select.h"

// Declaraciones externas de los recursos generados
extern const u32 gKantoRegionMapTiles[];
extern const u16 gKantoRegionMapPalette[];
extern const u32 gKantoRegionMapTilemap[];

// Dibuja el minimapa de Kanto en la ventana central del menú de selección de región
void DrawKantoMinimap(u8 windowId)
{
    // Cargar paleta
    LoadPalette(gKantoRegionMapPalette, BG_PLTT_ID(8), 32);
    // Cargar tiles en la VRAM (BG 0)
    CpuSet(gKantoRegionMapTiles, (void*)BG_CHAR_ADDR(2), 3200); // 10KB aprox
    // Cargar tilemap en la VRAM (BG 0)
    CpuSet(gKantoRegionMapTilemap, (void*)BG_SCREEN_ADDR(31), 1024); // 32x32
    // Opcional: dibujar un marco o overlay en la ventana
    // ...
}

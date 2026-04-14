#ifndef GUARD_REGION_SELECT_H
#define GUARD_REGION_SELECT_H

// Regiones disponibles para selección al inicio de nueva partida
#define REGION_HOENN    0
#define REGION_KANTO    1
#define REGION_JOHTO    2
#define REGION_COUNT    3

#define START_POINT_COUNT 3

// Variable global que almacena la región elegida por el jugador
extern u8 gSelectedRegion;
extern u8 gSelectedStartPoint;

void CB2_InitRegionSelect(void);
void SetWarpDestinationFromRegionSelection(void);

#endif // GUARD_REGION_SELECT_H

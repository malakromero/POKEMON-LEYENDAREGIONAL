#include "global.h"
#include "region_select.h"
#include "region_map.h" // For struct RegionMap and map functions
#include "char_customize.h"
#include "bg.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "sound.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "string_util.h"
#include "overworld.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/map_groups.h"
#include "constants/region_map_sections.h"

#define WIN_REGION_TITLE    0
#define WIN_REGION_LEFT     1
#define WIN_REGION_CENTER   2
#define WIN_REGION_RIGHT    3
#define WIN_REGION_HINT     4

#define STAGE_REGION        0
#define STAGE_STARTPOINT    1

#define tStage data[0]

#define REGION_MAP_CURSOR_X_MIN 1
#define REGION_MAP_CURSOR_Y_MIN 2
#define TAG_REGION_SELECT_CURSOR 3000

#define HOENN_BG_COLOR  RGB(14, 22, 31)
#define KANTO_BG_COLOR  RGB(10, 20, 10)
#define JOHTO_BG_COLOR  RGB(20, 15, 8)

EWRAM_DATA u8 gSelectedRegion = REGION_HOENN;
EWRAM_DATA u8 gSelectedStartPoint = 0;

static const u16 sRegionBgColors[REGION_COUNT] = {
    HOENN_BG_COLOR,
    KANTO_BG_COLOR,
    JOHTO_BG_COLOR,
};

static const u8 sText_RegionQuestion[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Que region deseas iniciar?");
static const u8 sText_StartPointQuestion[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Elige punto de partida");

static const u8 sText_Hoenn[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}HOENN");
static const u8 sText_Kanto[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}KANTO");
static const u8 sText_Johto[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}JOHTO");

static const u8 sText_HoennDesc[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Region del Prof. Birch");
static const u8 sText_KantoDesc[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Region del Prof. Oak");
static const u8 sText_JohtoDesc[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Region del Prof. Elm");

static const u8 sText_HintRegion[] = _("{COLOR LIGHT_GRAY}{SHADOW DARK_GRAY}IZQ/DER o B: Region  A: Confirmar");
static const u8 sText_HintStart[] = _("{COLOR LIGHT_GRAY}{SHADOW DARK_GRAY}DPAD: Cursor  A: Confirmar  B/SELECT: Atras");

static const u8 sText_Start_Hoenn0[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Littleroot");
static const u8 sText_Start_Hoenn1[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Oldale");
static const u8 sText_Start_Hoenn2[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Petalburg");
static const u8 sText_Start_Kanto0[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Pallet");
static const u8 sText_Start_Kanto1[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Viridian");
static const u8 sText_Start_Kanto2[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Pewter");
static const u8 sText_Start_Johto0[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}New Bark");
static const u8 sText_Start_Johto1[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Cherrygrove");
static const u8 sText_Start_Johto2[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Violet");
static const u8 sText_Arrows[] = _("{COLOR LIGHT_GRAY}{SHADOW DARK_GRAY}< >");

static const u8 *const sRegionNames[REGION_COUNT] = {
    sText_Hoenn,
    sText_Kanto,
    sText_Johto,
};

static const u8 *const sRegionDescs[REGION_COUNT] = {
    sText_HoennDesc,
    sText_KantoDesc,
    sText_JohtoDesc,
};

static const u8 *const sStartPointNames[REGION_COUNT][START_POINT_COUNT] = {
    [REGION_HOENN] = { sText_Start_Hoenn0, sText_Start_Hoenn1, sText_Start_Hoenn2 },
    [REGION_KANTO] = { sText_Start_Kanto0, sText_Start_Kanto1, sText_Start_Kanto2 },
    [REGION_JOHTO] = { sText_Start_Johto0, sText_Start_Johto1, sText_Start_Johto2 },
};

static const struct WindowTemplate sRegionSelectWinTemplates[] =
{
    [WIN_REGION_TITLE] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop  = 1,
        .width       = 28,
        .height      = 3,
        .paletteNum  = 15,
        .baseBlock   = 0x001,
    },
    [WIN_REGION_LEFT] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop  = 5,
        .width       = 7,
        .height      = 10,
        .paletteNum  = 15,
        .baseBlock   = 0x060,
    },
    [WIN_REGION_CENTER] = {
        .bg = 0,
        .tilemapLeft = 9,
        .tilemapTop  = 4,
        .width       = 12,
        .height      = 12,
        .paletteNum  = 15,
        .baseBlock   = 0x0A0,
    },
    [WIN_REGION_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 22,
        .tilemapTop  = 5,
        .width       = 7,
        .height      = 10,
        .paletteNum  = 15,
        .baseBlock   = 0x100,
    },
    [WIN_REGION_HINT] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop  = 17,
        .width       = 28,
        .height      = 2,
        .paletteNum  = 15,
        .baseBlock   = 0x140,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sRegionSelectBgTemplates[] =
{
    {
        .bg            = 2,
        .charBaseIndex = 2,
        .mapBaseIndex  = 28,
        .screenSize    = 2,
        .paletteMode   = 1,
        .priority      = 2,
        .baseTile      = 0,
    },
    {
        .bg            = 0,
        .charBaseIndex = 0,
        .mapBaseIndex  = 31,
        .screenSize    = 0,
        .paletteMode   = 0,
        .priority      = 3,
        .baseTile      = 0,
    },
};

static EWRAM_DATA struct RegionMap sRegionSelectMap;
static EWRAM_DATA bool8 sRegionMapReady = FALSE;

static void MainCB2_RegionSelect(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_RegionSelect(void)
{
    DoScheduledBgTilemapCopiesToVram();
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static u8 RegionPrev(u8 r)
{
    return (r == 0) ? REGION_COUNT - 1 : r - 1;
}

static u8 RegionNext(u8 r)
{
    return (r >= REGION_COUNT - 1) ? 0 : r + 1;
}

static u8 StartPrev(u8 s)
{
    return (s == 0) ? START_POINT_COUNT - 1 : s - 1;
}

static u8 StartNext(u8 s)
{
    return (s >= START_POINT_COUNT - 1) ? 0 : s + 1;
}

static mapsec_u16_t StartPointToMapSec(u8 region, u8 startPoint)
{
    if (region != REGION_HOENN)
        return MAPSEC_NONE;

    switch (startPoint)
    {
    case 1:
        return MAPSEC_OLDALE_TOWN;
    case 2:
        return MAPSEC_PETALBURG_CITY;
    case 0:
    default:
        return MAPSEC_LITTLEROOT_TOWN;
    }
}

static bool8 TrySetStartPointFromMapSec(mapsec_u16_t mapSecId)
{
    if (gSelectedRegion != REGION_HOENN)
        return FALSE;

    switch (mapSecId)
    {
    case MAPSEC_LITTLEROOT_TOWN:
        gSelectedStartPoint = 0;
        return TRUE;
    case MAPSEC_OLDALE_TOWN:
        gSelectedStartPoint = 1;
        return TRUE;
    case MAPSEC_PETALBURG_CITY:
        gSelectedStartPoint = 2;
        return TRUE;
    default:
        return FALSE;
    }
}

static void SetRegionCursorVisible(bool8 visible)
{
    if (sRegionMapReady && sRegionSelectMap.cursorSprite != NULL)
        sRegionSelectMap.cursorSprite->invisible = !visible;
}

static void MoveCursorToSelectedStartPoint(void)
{
    mapsec_u16_t mapSecId;
    const struct RegionMapLocation *loc;

    if (!sRegionMapReady)
        return;

    mapSecId = StartPointToMapSec(gSelectedRegion, gSelectedStartPoint);
    if (mapSecId == MAPSEC_NONE)
        return;

    loc = &gRegionMapEntries[mapSecId];
    sRegionSelectMap.mapSecId = mapSecId;
    sRegionSelectMap.cursorPosX = loc->x + (loc->width / 2) + REGION_MAP_CURSOR_X_MIN;
    sRegionSelectMap.cursorPosY = loc->y + (loc->height / 2) + REGION_MAP_CURSOR_Y_MIN;

    if (sRegionSelectMap.cursorSprite != NULL)
    {
        sRegionSelectMap.cursorSprite->x = 8 * sRegionSelectMap.cursorPosX + 4;
        sRegionSelectMap.cursorSprite->y = 8 * sRegionSelectMap.cursorPosY + 4;
    }
}

static void DrawRegionTitle(u8 stage)
{
    const u8 *text = (stage == STAGE_REGION) ? sText_RegionQuestion : sText_StartPointQuestion;
    const u8 *selectionText = (stage == STAGE_REGION)
        ? sRegionNames[gSelectedRegion]
        : sStartPointNames[gSelectedRegion][gSelectedStartPoint];

    FillWindowPixelBuffer(WIN_REGION_TITLE, PIXEL_FILL(1));
    FillWindowPixelRect(WIN_REGION_TITLE, PIXEL_FILL(3), 0, 0, 224, 2);
    AddTextPrinterParameterized(WIN_REGION_TITLE, FONT_NORMAL, text, 4, 8, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_REGION_TITLE, FONT_SMALL, selectionText, 132, 9, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_REGION_TITLE);
    CopyWindowToVram(WIN_REGION_TITLE, COPYWIN_FULL);
}

static void DrawRegionHint(u8 stage)
{
    const u8 *text = (stage == STAGE_REGION) ? sText_HintRegion : sText_HintStart;

    FillWindowPixelBuffer(WIN_REGION_HINT, PIXEL_FILL(1));
    FillWindowPixelRect(WIN_REGION_HINT, PIXEL_FILL(2), 0, 14, 224, 2);
    AddTextPrinterParameterized(WIN_REGION_HINT, FONT_SMALL, text, 2, 2, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_REGION_HINT);
    CopyWindowToVram(WIN_REGION_HINT, COPYWIN_FULL);
}

static void DrawRegionPanels(u8 stage)
{
    u8 leftRegion = RegionPrev(gSelectedRegion);
    u8 rightRegion = RegionNext(gSelectedRegion);
    const u8 *centerSubtitle;

    if (stage == STAGE_REGION)
        centerSubtitle = sRegionDescs[gSelectedRegion];
    else
        centerSubtitle = sStartPointNames[gSelectedRegion][gSelectedStartPoint];

    if (gSelectedRegion == REGION_HOENN)
    {
        ClearWindowTilemap(WIN_REGION_LEFT);
        ClearWindowTilemap(WIN_REGION_CENTER);
        ClearWindowTilemap(WIN_REGION_RIGHT);
        CopyWindowToVram(WIN_REGION_LEFT, COPYWIN_MAP);
        CopyWindowToVram(WIN_REGION_CENTER, COPYWIN_MAP);
        CopyWindowToVram(WIN_REGION_RIGHT, COPYWIN_MAP);
        return;
    }

    FillWindowPixelBuffer(WIN_REGION_LEFT, PIXEL_FILL(1));
    FillWindowPixelRect(WIN_REGION_LEFT, PIXEL_FILL(2), 0, 0, 56, 2);
    FillWindowPixelRect(WIN_REGION_LEFT, PIXEL_FILL(2), 0, 0, 2, 80);
    FillWindowPixelRect(WIN_REGION_LEFT, PIXEL_FILL(2), 54, 0, 2, 80);
    FillWindowPixelRect(WIN_REGION_LEFT, PIXEL_FILL(2), 0, 78, 56, 2);
    AddTextPrinterParameterized(WIN_REGION_LEFT, FONT_SMALL, sRegionNames[leftRegion], 6, 10, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_REGION_LEFT, FONT_SMALL, sRegionDescs[leftRegion], 2, 30, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_REGION_LEFT);
    CopyWindowToVram(WIN_REGION_LEFT, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_REGION_CENTER, PIXEL_FILL(1));
    FillWindowPixelRect(WIN_REGION_CENTER, PIXEL_FILL(3), 0, 0, 96, 3);
    FillWindowPixelRect(WIN_REGION_CENTER, PIXEL_FILL(3), 0, 0, 3, 96);
    FillWindowPixelRect(WIN_REGION_CENTER, PIXEL_FILL(3), 93, 0, 3, 96);
    FillWindowPixelRect(WIN_REGION_CENTER, PIXEL_FILL(3), 0, 93, 96, 3);
    FillWindowPixelRect(WIN_REGION_CENTER, PIXEL_FILL(2), 6, 22, 84, 1);

    {
        u8 xOff = GetStringCenterAlignXOffset(FONT_NORMAL, sRegionNames[gSelectedRegion], 96);
        AddTextPrinterParameterized(WIN_REGION_CENTER, FONT_NORMAL, sRegionNames[gSelectedRegion], xOff, 6, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_REGION_CENTER, FONT_NORMAL, centerSubtitle, 8, 30, TEXT_SKIP_DRAW, NULL);
    }
    AddTextPrinterParameterized(WIN_REGION_CENTER, FONT_NORMAL, sText_Arrows, 36, 74, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_REGION_CENTER);
    CopyWindowToVram(WIN_REGION_CENTER, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_REGION_RIGHT, PIXEL_FILL(1));
    FillWindowPixelRect(WIN_REGION_RIGHT, PIXEL_FILL(2), 0, 0, 56, 2);
    FillWindowPixelRect(WIN_REGION_RIGHT, PIXEL_FILL(2), 0, 0, 2, 80);
    FillWindowPixelRect(WIN_REGION_RIGHT, PIXEL_FILL(2), 54, 0, 2, 80);
    FillWindowPixelRect(WIN_REGION_RIGHT, PIXEL_FILL(2), 0, 78, 56, 2);
    AddTextPrinterParameterized(WIN_REGION_RIGHT, FONT_SMALL, sRegionNames[rightRegion], 6, 10, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_REGION_RIGHT, FONT_SMALL, sRegionDescs[rightRegion], 2, 30, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_REGION_RIGHT);
    CopyWindowToVram(WIN_REGION_RIGHT, COPYWIN_FULL);
}

static void RedrawAll(u8 stage)
{
    DrawRegionTitle(stage);
    DrawRegionPanels(stage);
    DrawRegionHint(stage);
}

static void Task_RegionSelect_FadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (sRegionMapReady)
        {
            FreeRegionMapIconResources();
            sRegionMapReady = FALSE;
        }
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_InitCharCustomize);
    }
}

static void Task_RegionSelect_Input(u8 taskId)
{
    u8 oldRegion = gSelectedRegion;
    u8 oldStart = gSelectedStartPoint;

    if (gTasks[taskId].tStage == STAGE_REGION)
    {
        if (JOY_NEW(DPAD_RIGHT) || JOY_REPEAT(DPAD_RIGHT)
         || JOY_NEW(DPAD_DOWN) || JOY_REPEAT(DPAD_DOWN)
         || JOY_NEW(R_BUTTON))
        {
            PlaySE(SE_SELECT);
            gSelectedRegion = RegionNext(gSelectedRegion);
            gSelectedStartPoint = 0;
        }
        else if (JOY_NEW(DPAD_LEFT) || JOY_REPEAT(DPAD_LEFT)
              || JOY_NEW(DPAD_UP) || JOY_REPEAT(DPAD_UP)
              || JOY_NEW(L_BUTTON))
        {
            PlaySE(SE_SELECT);
            gSelectedRegion = RegionPrev(gSelectedRegion);
            gSelectedStartPoint = 0;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gSelectedRegion = RegionPrev(gSelectedRegion);
            gSelectedStartPoint = 0;
        }

        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tStage = STAGE_STARTPOINT;
            if (gSelectedRegion == REGION_HOENN)
            {
                MoveCursorToSelectedStartPoint();
                SetRegionCursorVisible(TRUE);
            }
            RedrawAll(STAGE_STARTPOINT);
            return;
        }
    }
    else
    {
        if (gSelectedRegion == REGION_HOENN)
        {
            switch (DoRegionMapInputCallback())
            {
            case MAP_INPUT_MOVE_END:
                TrySetStartPointFromMapSec(sRegionSelectMap.mapSecId);
                DrawRegionTitle(STAGE_STARTPOINT);
                break;
            case MAP_INPUT_A_BUTTON:
                if (TrySetStartPointFromMapSec(sRegionSelectMap.mapSecId))
                {
                    PlaySE(SE_SELECT);
                    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
                    gTasks[taskId].func = Task_RegionSelect_FadeOut;
                }
                else
                {
                    PlaySE(SE_FAILURE);
                }
                return;
            case MAP_INPUT_B_BUTTON:
                PlaySE(SE_SELECT);
                gTasks[taskId].tStage = STAGE_REGION;
                SetRegionCursorVisible(FALSE);
                RedrawAll(STAGE_REGION);
                return;
            }
        }
        else
        {
            if (JOY_NEW(DPAD_RIGHT) || JOY_REPEAT(DPAD_RIGHT)
             || JOY_NEW(DPAD_DOWN) || JOY_REPEAT(DPAD_DOWN)
             || JOY_NEW(R_BUTTON))
            {
                PlaySE(SE_SELECT);
                gSelectedStartPoint = StartNext(gSelectedStartPoint);
            }
            else if (JOY_NEW(DPAD_LEFT) || JOY_REPEAT(DPAD_LEFT)
                  || JOY_NEW(DPAD_UP) || JOY_REPEAT(DPAD_UP)
                  || JOY_NEW(L_BUTTON))
            {
                PlaySE(SE_SELECT);
                gSelectedStartPoint = StartPrev(gSelectedStartPoint);
            }
            else if (JOY_NEW(B_BUTTON))
            {
                PlaySE(SE_SELECT);
                gSelectedStartPoint = StartPrev(gSelectedStartPoint);
            }
        }

        if (JOY_NEW(SELECT_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tStage = STAGE_REGION;
            SetRegionCursorVisible(FALSE);
            RedrawAll(STAGE_REGION);
            return;
        }

        if (gSelectedRegion != REGION_HOENN && JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_RegionSelect_FadeOut;
            return;
        }
    }

    if (oldRegion != gSelectedRegion)
        LoadPalette(&sRegionBgColors[gSelectedRegion], BG_PLTT_ID(0), 2);

    if (oldRegion != gSelectedRegion || oldStart != gSelectedStartPoint)
        DrawRegionPanels(gTasks[taskId].tStage);
}

static void Task_RegionSelect_FadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_RegionSelect_Input;
}

void SetWarpDestinationFromRegionSelection(void)
{
    // Nota: Johto aun no tiene mapas jugables definidos en map_groups,
    // de forma temporal se redirige a Kanto como placeholder seguro.
    if (gSelectedRegion == REGION_HOENN)
    {
        switch (gSelectedStartPoint)
        {
        case 0:
        default:
            SetWarpDestination(MAP_GROUP(MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F), MAP_NUM(MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F), WARP_ID_NONE, 4, 6);
            break;
        case 1:
            SetWarpDestination(MAP_GROUP(MAP_OLDALE_TOWN), MAP_NUM(MAP_OLDALE_TOWN), WARP_ID_NONE, 8, 8);
            break;
        case 2:
            SetWarpDestination(MAP_GROUP(MAP_PETALBURG_CITY), MAP_NUM(MAP_PETALBURG_CITY), WARP_ID_NONE, 8, 8);
            break;
        }
    }
    else if (gSelectedRegion == REGION_KANTO)
    {
        switch (gSelectedStartPoint)
        {
        case 0:
        default:
            SetWarpDestination(MAP_GROUP(MAP_PALLET_TOWN_PLAYERS_HOUSE_2F), MAP_NUM(MAP_PALLET_TOWN_PLAYERS_HOUSE_2F), WARP_ID_NONE, 6, 6);
            break;
        case 1:
            SetWarpDestination(MAP_GROUP(MAP_VIRIDIAN_CITY), MAP_NUM(MAP_VIRIDIAN_CITY), WARP_ID_NONE, 8, 8);
            break;
        case 2:
            SetWarpDestination(MAP_GROUP(MAP_PEWTER_CITY), MAP_NUM(MAP_PEWTER_CITY), WARP_ID_NONE, 8, 8);
            break;
        }
    }
    else
    {
        switch (gSelectedStartPoint)
        {
        case 0:
        default:
            SetWarpDestination(MAP_GROUP(MAP_PALLET_TOWN_PLAYERS_HOUSE_2F), MAP_NUM(MAP_PALLET_TOWN_PLAYERS_HOUSE_2F), WARP_ID_NONE, 6, 6);
            break;
        case 1:
            SetWarpDestination(MAP_GROUP(MAP_VIRIDIAN_CITY), MAP_NUM(MAP_VIRIDIAN_CITY), WARP_ID_NONE, 8, 8);
            break;
        case 2:
            SetWarpDestination(MAP_GROUP(MAP_PEWTER_CITY), MAP_NUM(MAP_PEWTER_CITY), WARP_ID_NONE, 8, 8);
            break;
        }
    }
}

void CB2_InitRegionSelect(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sRegionSelectBgTemplates, ARRAY_COUNT(sRegionSelectBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        InitWindows(sRegionSelectWinTemplates);
        DeactivateAllTextPrinters();
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadPalette(&sRegionBgColors[gSelectedRegion], BG_PLTT_ID(0), 2);
        {
            static const u16 sTxtPal[] = {
                RGB( 0,  0,  0),
                RGB( 4,  4,  8),
                RGB(14, 14, 16),
                RGB(31, 26, 10),
                RGB(31, 31, 31),
            };
            LoadPalette(sTxtPal, BG_PLTT_ID(15), sizeof(sTxtPal));
        }
        gMain.state++;
        break;
    case 4:
        ShowBg(0);
        ShowBg(2);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON);
        gMain.state++;
        break;
    case 5:
        InitRegionMapData(&sRegionSelectMap, &sRegionSelectBgTemplates[0], FALSE);
        while (LoadRegionMapGfx())
            ;
        CreateRegionMapCursor(TAG_REGION_SELECT_CURSOR, TAG_REGION_SELECT_CURSOR);
        sRegionMapReady = TRUE;
        SetRegionCursorVisible(FALSE);
        RedrawAll(STAGE_REGION);
        gMain.state++;
        break;
    case 6:
    {
        u8 taskId;

        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_RegionSelect);
        SetMainCallback2(MainCB2_RegionSelect);
        taskId = CreateTask(Task_RegionSelect_FadeIn, 0);
        gTasks[taskId].tStage = STAGE_REGION;
        break;
    }
    }
}

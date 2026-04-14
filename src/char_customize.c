#include "global.h"
#include "char_customize.h"
//#include "fondo_azul_pokeball.c"
#include "main_menu.h"
#include "fondo_azul_pokeball.h"
#include "region_select.h"
#include "trainer_pokemon_sprites.h"
#include "bg.h"
#include "field_effect.h"
#include "gpu_regs.h"
#include "main.h"
#include "menu.h"
#include "naming_screen.h"
#include "palette.h"
#include "pokemon.h"
#include "random.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"

#define WIN_TITLE   0
#define WIN_MENU    1
#define WIN_HINT    2

#define PREVIEW_X   182
#define PREVIEW_Y   88

#define ITEM_SPRITE     0
#define ITEM_GENDER     1
#define ITEM_SKIN       2
#define ITEM_OUTFIT     3
#define ITEM_NAME       4
#define ITEM_RANDOM     5
#define ITEM_SAVE       6
#define ITEM_COUNT      7

#define tSelection data[0]
#define tSpriteId0 data[2]
#define tSpriteId1 data[3]
#define tSpriteId2 data[4]
#define tSpriteId3 data[5]

struct CharCustomization gCharCustomization = {
    .spriteIndex = CHAR_SPRITE_BRENDAN,
    .skinColor = SKIN_COLOR_LIGHT,
    .outfitColor = OUTFIT_COLOR_DEFAULT,
    .gender = MALE,
};

EWRAM_DATA bool8 gSkipBirchPlayerSetup = FALSE;

static EWRAM_DATA bool8 sReturnFromNaming = FALSE;
static EWRAM_DATA u16 sBaseObjPalettes[CHAR_SPRITE_COUNT][16] = {0};
static EWRAM_DATA u8 sPaletteSlots[CHAR_SPRITE_COUNT] = {0};
static EWRAM_DATA u8 sCharCustomizeTaskId = 0;

static const u8 sText_Title[] = _("{COLOR WHITE}{SHADOW DARK_GRAY}Personalizacion de personaje");
static const u8 sText_Hint[] = _("{COLOR LIGHT_GRAY}{SHADOW DARK_GRAY}ARR/ABAJO: Opcion  IZQ/DER: Cambiar");

static const u8 sText_Items[ITEM_COUNT][16] = {
    _("Sprite"),
    _("Sexo"),
    _("Piel"),
    _("Ropa"),
    _("Nombre"),
    _("Aleatorio"),
    _("Guardar"),
};

static const u8 sText_Sprite_Brendan[] = _("Brendan");
static const u8 sText_Sprite_May[] = _("May");
static const u8 sText_Sprite_Red[] = _("Red");
static const u8 sText_Sprite_Leaf[] = _("Leaf");

static const u8 sText_Gender_Male[] = _("Masculino");
static const u8 sText_Gender_Female[] = _("Femenino");

static const u8 sText_Skin_Light[] = _("Claro");
static const u8 sText_Skin_Medium[] = _("Medio");
static const u8 sText_Skin_Dark[] = _("Oscuro");

static const u8 sText_Outfit_Default[] = _("Base");
static const u8 sText_Outfit_Red[] = _("Rojo");
static const u8 sText_Outfit_Blue[] = _("Azul");
static const u8 sText_Outfit_Green[] = _("Verde");
static const u8 sText_Outfit_Yellow[] = _("Amarillo");

static const u8 *const sSpriteNames[CHAR_SPRITE_COUNT] = {
    sText_Sprite_Brendan,
    sText_Sprite_May,
    sText_Sprite_Red,
    sText_Sprite_Leaf,
};

static const u8 *const sSkinNames[SKIN_COLOR_COUNT] = {
    sText_Skin_Light,
    sText_Skin_Medium,
    sText_Skin_Dark,
};

static const u8 *const sOutfitNames[OUTFIT_COLOR_COUNT] = {
    sText_Outfit_Default,
    sText_Outfit_Red,
    sText_Outfit_Blue,
    sText_Outfit_Green,
    sText_Outfit_Yellow,
};

static const struct WindowTemplate sCharCustomizeWinTemplates[] =
{
    [WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 28,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x258, // Movido después de los 600 tiles de tu fondo
    },
    [WIN_MENU] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 4,
        .width = 20,
        .height = 14,
        .paletteNum = 15,
        .baseBlock = 0x60,
    },
    [WIN_HINT] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 18,
        .width = 28,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x140,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 1,
        .priority = 3,
        .baseTile = 0,
    },
};

static const u16 sBgPal[] = { RGB(8, 10, 16) };

static void MainCB2_CharCustomize(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_CharCustomize(void)
{
    DoScheduledBgTilemapCopiesToVram();
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static u8 NextIndex(u8 idx, u8 count)
{
    idx++;
    if (idx >= count)
        idx = 0;
    return idx;
}

static u8 PrevIndex(u8 idx, u8 count)
{
    if (idx == 0)
        idx = count - 1;
    else
        idx--;
    return idx;
}

static u16 BlendTowardColor(u16 c, u16 target, u8 weight)
{
    u8 r = c & 31;
    u8 g = (c >> 5) & 31;
    u8 b = (c >> 10) & 31;

    u8 tr = target & 31;
    u8 tg = (target >> 5) & 31;
    u8 tb = (target >> 10) & 31;

    r = (r * (16 - weight) + tr * weight) / 16;
    g = (g * (16 - weight) + tg * weight) / 16;
    b = (b * (16 - weight) + tb * weight) / 16;

    return RGB(r, g, b);
}

static void ApplyPreviewPalette(u8 spriteIndex)
{
    u8 i;
    u8 palSlot = sPaletteSlots[spriteIndex];
    u16 *objPal = &gPlttBufferUnfaded[OBJ_PLTT_ID(palSlot)];
    const u16 *basePal = sBaseObjPalettes[spriteIndex];
    u16 skinTint;
    u16 outfitTint;

    for (i = 0; i < 16; i++)
        objPal[i] = basePal[i];

    switch (gCharCustomization.skinColor)
    {
    case SKIN_COLOR_LIGHT:
    default:
        skinTint = RGB(31, 26, 20);
        break;
    case SKIN_COLOR_MEDIUM:
        skinTint = RGB(25, 18, 12);
        break;
    case SKIN_COLOR_DARK:
        skinTint = RGB(16, 11, 8);
        break;
    }

    switch (gCharCustomization.outfitColor)
    {
    case OUTFIT_COLOR_DEFAULT:
    default:
        outfitTint = RGB(15, 15, 15);
        break;
    case OUTFIT_COLOR_RED:
        outfitTint = RGB(28, 8, 8);
        break;
    case OUTFIT_COLOR_BLUE:
        outfitTint = RGB(8, 12, 28);
        break;
    case OUTFIT_COLOR_GREEN:
        outfitTint = RGB(8, 24, 10);
        break;
    case OUTFIT_COLOR_YELLOW:
        outfitTint = RGB(30, 24, 8);
        break;
    }

    // Ajustes simples sobre indices comunes para piel/ropa.
    for (i = 9; i <= 11; i++)
        objPal[i] = BlendTowardColor(objPal[i], skinTint, 10);

    for (i = 4; i <= 6; i++)
        objPal[i] = BlendTowardColor(objPal[i], outfitTint, 10);

    CpuCopy16(objPal, &gPlttBufferFaded[OBJ_PLTT_ID(palSlot)], 16 * sizeof(u16));
}

static void SetPreviewSpriteVisibility(u8 selected)
{
    gSprites[gTasks[sCharCustomizeTaskId].tSpriteId0].invisible = (selected != CHAR_SPRITE_BRENDAN);
    gSprites[gTasks[sCharCustomizeTaskId].tSpriteId1].invisible = (selected != CHAR_SPRITE_MAY);
    gSprites[gTasks[sCharCustomizeTaskId].tSpriteId2].invisible = (selected != CHAR_SPRITE_RED);
    gSprites[gTasks[sCharCustomizeTaskId].tSpriteId3].invisible = (selected != CHAR_SPRITE_LEAF);
}

static void DrawStaticWindows(void)
{
    FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(1));
    FillWindowPixelRect(WIN_TITLE, PIXEL_FILL(3), 0, 0, 224, 2);
    AddTextPrinterParameterized(WIN_TITLE, FONT_NORMAL, sText_Title, 4, 2, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_TITLE);
    CopyWindowToVram(WIN_TITLE, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HINT, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_HINT, FONT_SMALL, sText_Hint, 2, 2, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HINT);
    CopyWindowToVram(WIN_HINT, COPYWIN_FULL);
}

static void DrawMenu(u8 selection)
{
    u8 i;
    u8 y = 4;
    const u8 *genderName = (gCharCustomization.gender == MALE) ? sText_Gender_Male : sText_Gender_Female;

    FillWindowPixelBuffer(WIN_MENU, PIXEL_FILL(1));

    for (i = 0; i < ITEM_COUNT; i++)
    {
        if (i == selection)
            FillWindowPixelRect(WIN_MENU, PIXEL_FILL(3), 0, y - 1, 160, 12);

        AddTextPrinterParameterized(WIN_MENU, FONT_SMALL, sText_Items[i], 4, y, TEXT_SKIP_DRAW, NULL);

        switch (i)
        {
        case ITEM_SPRITE:
            AddTextPrinterParameterized(WIN_MENU, FONT_SMALL, sSpriteNames[gCharCustomization.spriteIndex], 78, y, TEXT_SKIP_DRAW, NULL);
            break;
        case ITEM_GENDER:
            AddTextPrinterParameterized(WIN_MENU, FONT_SMALL, genderName, 78, y, TEXT_SKIP_DRAW, NULL);
            break;
        case ITEM_SKIN:
            AddTextPrinterParameterized(WIN_MENU, FONT_SMALL, sSkinNames[gCharCustomization.skinColor], 78, y, TEXT_SKIP_DRAW, NULL);
            break;
        case ITEM_OUTFIT:
            AddTextPrinterParameterized(WIN_MENU, FONT_SMALL, sOutfitNames[gCharCustomization.outfitColor], 78, y, TEXT_SKIP_DRAW, NULL);
            break;
        case ITEM_NAME:
            AddTextPrinterParameterized(WIN_MENU, FONT_SMALL, gSaveBlock2Ptr->playerName, 78, y, TEXT_SKIP_DRAW, NULL);
            break;
        default:
            break;
        }

        y += 16;
    }

    PutWindowTilemap(WIN_MENU);
    CopyWindowToVram(WIN_MENU, COPYWIN_FULL);
}

static void ApplyAllPreviewVisuals(void)
{
    SetPreviewSpriteVisibility(gCharCustomization.spriteIndex);
    ApplyPreviewPalette(gCharCustomization.spriteIndex);
}

static void CB2_ReturnFromNaming(void)
{
    sReturnFromNaming = TRUE;
    SetMainCallback2(CB2_InitCharCustomize);
}

static void Task_CharCustomize_FadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_StartNewGameBirchSpeech);
    }
}

static void Task_CharCustomize_FadeOutToRegionSelect(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_InitRegionSelect);
    }
}

static void Task_CharCustomize_Input(u8 taskId)
{
    u8 oldSprite = gCharCustomization.spriteIndex;
    u8 oldSkin = gCharCustomization.skinColor;
    u8 oldOutfit = gCharCustomization.outfitColor;

    if (JOY_NEW(DPAD_UP) || JOY_REPEAT(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tSelection = PrevIndex(gTasks[taskId].tSelection, ITEM_COUNT);
        DrawMenu(gTasks[taskId].tSelection);
        return;
    }

    if (JOY_NEW(DPAD_DOWN) || JOY_REPEAT(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tSelection = NextIndex(gTasks[taskId].tSelection, ITEM_COUNT);
        DrawMenu(gTasks[taskId].tSelection);
        return;
    }

    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_CharCustomize_FadeOutToRegionSelect;
        return;
    }

    if (JOY_NEW(DPAD_LEFT) || JOY_REPEAT(DPAD_LEFT) || JOY_NEW(L_BUTTON))
    {
        switch (gTasks[taskId].tSelection)
        {
        case ITEM_SPRITE:
            PlaySE(SE_SELECT);
            gCharCustomization.spriteIndex = PrevIndex(gCharCustomization.spriteIndex, CHAR_SPRITE_COUNT);
            break;
        case ITEM_GENDER:
            PlaySE(SE_SELECT);
            gCharCustomization.gender = (gCharCustomization.gender == MALE) ? FEMALE : MALE;
            break;
        case ITEM_SKIN:
            PlaySE(SE_SELECT);
            gCharCustomization.skinColor = PrevIndex(gCharCustomization.skinColor, SKIN_COLOR_COUNT);
            break;
        case ITEM_OUTFIT:
            PlaySE(SE_SELECT);
            gCharCustomization.outfitColor = PrevIndex(gCharCustomization.outfitColor, OUTFIT_COLOR_COUNT);
            break;
        default:
            break;
        }
    }

    if (JOY_NEW(DPAD_RIGHT) || JOY_REPEAT(DPAD_RIGHT) || JOY_NEW(R_BUTTON))
    {
        switch (gTasks[taskId].tSelection)
        {
        case ITEM_SPRITE:
            PlaySE(SE_SELECT);
            gCharCustomization.spriteIndex = NextIndex(gCharCustomization.spriteIndex, CHAR_SPRITE_COUNT);
            break;
        case ITEM_GENDER:
            PlaySE(SE_SELECT);
            gCharCustomization.gender = (gCharCustomization.gender == MALE) ? FEMALE : MALE;
            break;
        case ITEM_SKIN:
            PlaySE(SE_SELECT);
            gCharCustomization.skinColor = NextIndex(gCharCustomization.skinColor, SKIN_COLOR_COUNT);
            break;
        case ITEM_OUTFIT:
            PlaySE(SE_SELECT);
            gCharCustomization.outfitColor = NextIndex(gCharCustomization.outfitColor, OUTFIT_COLOR_COUNT);
            break;
        default:
            break;
        }
    }

    if (JOY_NEW(A_BUTTON))
    {
        switch (gTasks[taskId].tSelection)
        {
        case ITEM_SPRITE:
            PlaySE(SE_SELECT);
            gCharCustomization.spriteIndex = NextIndex(gCharCustomization.spriteIndex, CHAR_SPRITE_COUNT);
            break;
        case ITEM_GENDER:
            PlaySE(SE_SELECT);
            gCharCustomization.gender = (gCharCustomization.gender == MALE) ? FEMALE : MALE;
            break;
        case ITEM_SKIN:
            PlaySE(SE_SELECT);
            gCharCustomization.skinColor = NextIndex(gCharCustomization.skinColor, SKIN_COLOR_COUNT);
            break;
        case ITEM_OUTFIT:
            PlaySE(SE_SELECT);
            gCharCustomization.outfitColor = NextIndex(gCharCustomization.outfitColor, OUTFIT_COLOR_COUNT);
            break;
        case ITEM_NAME:
            PlaySE(SE_SELECT);
            DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gCharCustomization.gender, 0, 0, CB2_ReturnFromNaming);
            return;
        case ITEM_RANDOM:
            PlaySE(SE_SELECT);
            gCharCustomization.spriteIndex = Random() % CHAR_SPRITE_COUNT;
            gCharCustomization.gender = Random() & 1;
            gCharCustomization.skinColor = Random() % SKIN_COLOR_COUNT;
            gCharCustomization.outfitColor = Random() % OUTFIT_COLOR_COUNT;
            break;
        case ITEM_SAVE:
            PlaySE(SE_SELECT);
            gSaveBlock2Ptr->playerGender = gCharCustomization.gender;
            gSaveBlock2Ptr->playerSpriteIndex = gCharCustomization.spriteIndex;
            gSkipBirchPlayerSetup = TRUE;
            if (gSaveBlock2Ptr->playerName[0] == EOS)
                NewGameBirchSpeech_SetDefaultPlayerName(gSaveBlock2Ptr->playerGender);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_CharCustomize_FadeOut;
            return;
        default:
            break;
        }
    }

    if (oldSprite != gCharCustomization.spriteIndex
     || oldSkin != gCharCustomization.skinColor
     || oldOutfit != gCharCustomization.outfitColor)
    {
        ApplyAllPreviewVisuals();
    }

    DrawMenu(gTasks[taskId].tSelection);
}

static void Task_CharCustomize_FadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_CharCustomize_Input;
}

void CB2_InitCharCustomize(void)
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
        InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        InitWindows(sCharCustomizeWinTemplates);
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
    {
        // 1. Cargar Paleta completa (256 colores)
        LoadPalette(fondo_azul_pokeballPal, BG_PLTT_ID(0), 512);

        // 2. Cargar los tiles (38400 bytes es el tamaño de tus 9600 ints)
        LoadBgTiles(0, fondo_azul_pokeballTiles, 38400, 0);

        // 3. Limpiar el mapa del BG0 por seguridad
        FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);

        // 4. Cargar tu Mapa (los 600 números)
        CopyToBgTilemapBuffer(0, fondo_azul_pokeballMap, 0, 0);

        // 5. Enviar a VRAM
        CopyBgTilemapBufferToVram(0);

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
        // ... resto del código ...

        taskId = CreateTask(Task_CharCustomize_FadeIn, 0);
        sCharCustomizeTaskId = taskId;
        gTasks[taskId].tSelection = 0;

        gTasks[taskId].tSpriteId0 = CreateTrainerSprite(FacilityClassToPicIndex(FACILITY_CLASS_BRENDAN), PREVIEW_X, PREVIEW_Y, 0, NULL);
        gTasks[taskId].tSpriteId1 = CreateTrainerSprite(FacilityClassToPicIndex(FACILITY_CLASS_MAY), PREVIEW_X, PREVIEW_Y, 0, NULL);
        gTasks[taskId].tSpriteId2 = CreateTrainerSprite(FacilityClassToPicIndex(FACILITY_CLASS_RED), PREVIEW_X, PREVIEW_Y, 0, NULL);
        gTasks[taskId].tSpriteId3 = CreateTrainerSprite(FacilityClassToPicIndex(FACILITY_CLASS_LEAF), PREVIEW_X, PREVIEW_Y, 0, NULL);

        for (i = 0; i < CHAR_SPRITE_COUNT; i++)
        {
            u8 spriteId = gTasks[taskId].data[2 + i];
            u8 palSlot = gSprites[spriteId].oam.paletteNum;
            sPaletteSlots[i] = palSlot;
            CpuCopy16(&gPlttBufferUnfaded[OBJ_PLTT_ID(palSlot)], sBaseObjPalettes[i], 16 * sizeof(u16));
            gSprites[spriteId].oam.priority = 0;
            gSprites[spriteId].invisible = TRUE;
        }

        DrawStaticWindows();
        DrawMenu(gTasks[taskId].tSelection);
        ApplyAllPreviewVisuals();

        ShowBg(0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_CharCustomize);
        SetMainCallback2(MainCB2_CharCustomize);
        sReturnFromNaming = FALSE;
        break;
    }
    }
}

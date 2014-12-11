#ifndef __GAME__
#define __GAME__

#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "fighter.h"
#include "stage.h"
#include "menus.h"
#include <glib.h>

typedef enum{
	MAIN_MENU,
	C_SELECT,
	S_SELECT,
	OPTIONS,
	VERSUS,
	C_CREATOR
}GameState_T;

void Init_All();
void CleanUpAll();
void DrawUpdate();
void DrawNextRoundTimer(int time);
void InitVersus();
void UpdateVersus();
void NextRound();
void NextGame();
void GamePause();
void GoToMenu();
void DrawMainMenu();
void GoToVersus();
void DrawVersus();
void GoToCharSel();
void DrawCharSel();
void GoToStageSel();
void DrawStageSel();
void GoToCharCr();
void InitCharCr();
void DrawCharCr();
void UpdateCharCr();

void InitFont();


#endif
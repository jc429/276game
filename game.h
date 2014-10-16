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
	PAUSE
}GameState_T;

GameState_T GameState;
void Init_All();
void CleanUpAll();
void DrawUpdate();
void DrawNextRoundTimer();
void InitVersus();

#endif
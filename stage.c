#include "stage.h"
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"

extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Surface *screen; /*pointer to the screen*/
int x = 0;

int STAGEFLOOR,STAGELEFT,STAGERIGHT,P1SPAWN,P2SPAWN;

void LoadStage(StageList stage){
	char* bg;
	switch(stage){
		case ST_PLATFORM:
			bg = "images/bgimage1.png";
			STAGEFLOOR = 540;
			STAGELEFT = 220;
			STAGERIGHT = 790;
			P1SPAWN = 270;
			P2SPAWN = 740;
			break;
		case ST_DEBOOG:
			bg = "images/bgtest.png";
			STAGEFLOOR = 550;
			STAGELEFT = 0;
			STAGERIGHT = 1025;
			P1SPAWN = 200;
			P2SPAWN = 820;
			break;
		case ST_FIELD:
			bg = "images/bgtest.png";
			STAGEFLOOR = 550;
			STAGELEFT = 0;
			STAGERIGHT = 1025;
			P1SPAWN = 200;
			P2SPAWN = 820;
			break;
	}

	DrawBG(bg);
	
}

void UpdateStage(){
}



void DrawStage(StageList stage){
	Sprite *fg=NULL;
	if(stage == ST_PLATFORM)
		fg = LoadSprite("images/stage.png",1024,768, 1);
	if(fg!=NULL)
		DrawSprite(fg,screen,0,0,0);
	
}
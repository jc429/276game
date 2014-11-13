#include "stage.h"
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
//#include <chipmunk\cpSpace.h>
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Surface *screen; /*pointer to the screen*/
int x = 0;
//cpSpace* space = NULL;

//int STAGEFLOOR,STAGELEFT,STAGERIGHT,P1SPAWN,P2SPAWN;
Stage st; 
Platform platform;
StageLayer layer;
void LoadStage(StageList stagesel){
	switch(stagesel){
		case ST_PLATFORM:
			st.bg = "images/bgimage1.png";
			
			platform.p_ypos = 540;
			platform.p_left = 220;
			platform.p_right = 790;
			
			st.P1spawn= 270;
			st.P2spawn = 740;
			break;
		case ST_DEBUG:
		case ST_FIELD:
			st.bg = "images/bgtest.png";
			platform.p_ypos = 550;
			platform.p_left = 0;
			platform.p_right = 1025;
			st.P1spawn = 200;
			st.P2spawn = 820;
			break;
	}
	st.platform_list = &platform;
	st.layer_list = &layer;
	DrawBG(st.bg);
	
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
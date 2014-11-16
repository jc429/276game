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
			layer.layerpath = "images/bgimage2.png";
				layer.movescale = 1;
				layer.w = 1536;
				layer.h = 1152;
				layer.offx = layer.w*0.5;
				layer.offy = layer.h*0.5;
				layer.x = 512;
				layer.y = 768*0.5;

			platform.p_ypos = 540;
			platform.p_left = 220;
			platform.p_right = 790;
			platform.next=NULL;
			//	platform.next = (Platform*)malloc(sizeof(Platform));
		//	platform.next->p_ypos = 740;
		//	platform.next->p_left = 0;
		//	platform.next->p_right = 1024;
		//	platform.next->next=NULL;
			
			st.P1spawn= 270;
			st.P2spawn = 740;
			break;
		case ST_DEBUG:
		case ST_FIELD:
			st.bg = "images/bgtest.png";
			platform.p_ypos = 550;
			platform.p_left = 0;
			platform.p_right = 1025;
			layer.layerpath = NULL;
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

	if(st.layer_list->layerpath!=NULL){
		DrawLayer(st.layer_list);
		
	st.layer_list->x++;
	}

}

void DrawLayer(StageLayer* layer){
	Sprite* sp = LoadSprite(layer->layerpath,layer->w,layer->h, 1);
		
	DrawSprite(sp,screen,layer->x-layer->offx,layer->y-layer->offy,0);
	
}
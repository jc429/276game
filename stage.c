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
StageLayer* layer;
void LoadStage(Stage_T stagesel){
	
	layer = (StageLayer*)malloc(sizeof(StageLayer));
	switch(stagesel){
		case ST_PLATFORM:
			st.bg = "images/bgimage1.png";
			SetLayer(layer,1,512,384,1536,1152,768,576,"images/bgimage2.png",1);
			SetLayer(layer->next,2,512,384,1536,1152,768,576,"images/bgimage2.png",1);
			SetLayer(layer->next->next,0,0,0,1024,768,0,0,"images/stage.png",0);
			
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
			platform.p_ypos = 600;
			platform.p_left = 0;
			platform.p_right = 1025;
			SetLayer(layer,0,512,660,1722,162,861,81,"images/train.png",0);
			
			st.P1spawn = 200;
			st.P2spawn = 820;
			break;
	}
	st.platform_list = &platform;
	st.layer_list = layer;
	DrawBG(st.bg);
	
}

void SetLayer(StageLayer* layer,float movescale,int x, int y, int w, int h, int offx, int offy, char* path,int hasnext){
	layer->movescale = movescale;
	layer->x = x;
	layer->y = y;
	layer->w = w;
	layer->h = h;
	layer->offx = offx;
	layer->offy = offy;
	layer->layerpath = path;
	layer->next = NULL;
	if(hasnext!=0)
		layer->next = (StageLayer*)malloc(sizeof(StageLayer));
}

void UpdateStage(Stage_T stage){
	if(st.layer_list->layerpath!=NULL){
		UpdateLayer(st.layer_list);
	}
}

void UpdateLayer(StageLayer* layer){
	layer->x += layer->movescale;
	if(layer->next!=NULL)
		UpdateLayer(layer->next);
}

void DrawStage(Stage_T stage){

	if(st.layer_list->layerpath!=NULL){
		DrawLayer(st.layer_list);
	}

}

void DrawLayer(StageLayer* layer){
	Sprite* sp = LoadSprite(layer->layerpath,layer->w,layer->h, 1);		
	DrawSprite(sp,screen,layer->x-layer->offx,layer->y-layer->offy,0);
	if(layer->next!=NULL)
		DrawLayer(layer->next);
}
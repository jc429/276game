#include "stage.h"
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"

extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Surface *screen; /*pointer to the screen*/
int x = 0;
void LoadLevel(){
	
}

void UpdateStage(){
	x+=3;
	if(x>=1024) x=0;
}

void DrawStageBG(){
	SDL_Surface *temp;
	SDL_Surface *bg;
	temp = IMG_Load("images/bgtest2.png");/*notice that the path is part of the filename*/
	if(temp != NULL)						/*ALWAYS check your pointers before you use them*/
		bg = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if(bg != NULL)
		SDL_BlitSurface(bg,NULL,buffer,NULL);
}

void DrawStage(){
	
	Sprite *temp = LoadSprite("images/stage.png",1024,768, 1);
	DrawSprite(temp,screen,0,0,0);
/*	int width = 1024;
	Sprite *temp = LoadSprite("images/bgtest2-2.png",width,128, 1);

	DrawSprite(temp,screen,x-width,400,0);
	DrawSprite(temp,screen,x,400,0);
	DrawSprite(temp,screen,x+width,400,0);
*/
}
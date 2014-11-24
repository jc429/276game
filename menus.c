#include "menus.h"
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"

extern SDL_Surface *buffer,*screen;
Button testbutton;
TTF_Font *font;
SDL_Surface *textsurf;
void InitMenu(){
	DrawMenuBG();
	Sprite* spr = LoadSprite("images/button.png",200,60,1);
	SetButton(&testbutton,0,-1,"hello",spr,spr,spr,spr,500,400,200,60,1,0,100,111,0,0);
	
}

void UpdateMenu(){
	int mx,my;
	if(SDL_GetMouseState(&mx,&my))
    {
		DrawPixel(buffer,255,250,0,mx,my);
    }else{
		DrawPixel(buffer,0,0,0,mx,my);

	}
}

void DrawMenus(){
	DrawSprite(testbutton.button,screen,0,0,0);
	font = TTF_OpenFont("fonts/font.ttf",16);
	SDL_Color text_color = {255, 255, 255};
	textsurf = TTF_RenderText_Solid(font,"Hello buddy",text_color);
	SDL_BlitSurface(textsurf,NULL,buffer,NULL);
}

void DrawMenuBG(){
	char* menuBG = "images/menubg.png";
	DrawBG(menuBG);
}

void SetButton(Button *button,int buttonID,int hotkey, char *text,Sprite *sprite,Sprite *sprite1,Sprite *sprite2,Sprite *sprite3,int x,int y,int w,int h,int shown,int frame,int c1, int c2, int font,int centered)
{
  strcpy(button->text,text);
  button->button = sprite;
  button->hotkey = hotkey;
  button->buttons[0] = sprite1;
  button->buttons[1] = sprite2;
  button->buttons[2] = sprite3;
  button->buttonID = buttonID;
  button->frame = frame;
  button->box.x = x;
  button->box.y = y;
  button->box.w = w;
  button->box.h = h;
  button->shown = shown;
  button->font = font;
  button->color = c1;
  button->hcolor = c2;
  button->centered = centered;
}
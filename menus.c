#include "menus.h"
#include "SDL.h"
#include "SDL_image.h"
#include <SDL_ttf.h>
#include "graphics.h"
#include "game.h"

extern SDL_Surface *buffer,*screen;
Button testbutton, chrbutton;
void InitMenu(){
	DrawMenuBG();
	Sprite* spr = LoadSprite("images/buttonvs.png",200,60,1);
	SetButton(&testbutton,0,VersusClick,-1,"hello",spr,spr,spr,spr,400,400,200,60,1,0,100,111,0,0);
	spr = LoadSprite("images/buttoncc.png",200,60,1);
	SetButton(&chrbutton,0,ChrCrClick,-1,"hello",spr,spr,spr,spr,400,500,200,60,1,0,100,111,0,0);
	
}
void VersusClick(int a,...){
	GoToVersus();
}
void ChrCrClick(int a,...){
	GoToCharCr();
}

void UpdateMenu(){
	int mx,my;
	if(SDL_GetMouseState(&mx,&my))
    {
		if(MouseInButton(mx,my,&testbutton))
			testbutton.onClick(0);
		else if(MouseInButton(mx,my,&chrbutton))
			chrbutton.onClick(0);
    }else{
		DrawPixel(buffer,0,0,0,mx,my);

	}
}
extern SDL_Surface *textsurf;
extern TTF_Font *font;
void DrawMenus(){
	DrawSprite(testbutton.button,screen,testbutton.box.x,testbutton.box.y,0);
	DrawSprite(chrbutton.button,screen,chrbutton.box.x,chrbutton.box.y,0);
	
	
	SDL_Color bg_color = {0,0,0};
	
	DrawText("yeah buddy",400,43);
	
	
}

void DrawMenuBG(){
	char* menuBG = "images/menubg.png";
	DrawBG(menuBG);
}

void DrawText(char* message,int x, int y){
	SDL_Color text_color = {255, 255, 255};
	textsurf = TTF_RenderText_Solid(font,message,text_color);
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_BlitSurface(textsurf,NULL, screen, &dest);	
}

void SetButton(Button *button,int buttonID,void (*onClick)(int a,...),int hotkey, char *text,Sprite *sprite,Sprite *sprite1,Sprite *sprite2,Sprite *sprite3,int x,int y,int w,int h,int shown,int frame,int c1, int c2, int font,int centered)
{
  strcpy(button->text,text);
  button->button = sprite;
  button->onClick = onClick;
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

void DrawButton(Button* b){
	DrawSprite(b->button,screen,b->box.x,b->box.y,0);
	DrawText(b->text,b->box.x+5,b->box.y+5);
}

int MouseInButton(int mx, int my, Button *b){
	if((mx > b->box.x)&&(mx < (b->box.x+b->box.w))&&(my > b->box.y)&&(my < (b->box.y+b->box.h)))
		return 1;
	return 0;
}
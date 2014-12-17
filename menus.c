#include "menus.h"
#include "SDL.h"
#include "SDL_image.h"
#include <SDL_ttf.h>
#include "graphics.h"
#include "game.h"

extern SDL_Surface *buffer,*screen;
Button b1,b2,b3,b4,b5,b6,b7,b8; /* TODO: replace with a dynamic list of buttons*/
Button testbutton, chrbutton;
Mouse mouse;
void UpdateMouse(){
	mouse.clicked=0;
	mouse.held=0;
	mouse.released=0;
	if(SDL_GetMouseState(&mouse.x,&mouse.y)){
		if(mouse.prev==0){
			mouse.clicked=1;
		}else if(mouse.prev==1){
			mouse.held=1;
		}
		mouse.prev = 1;
	}else{
		if(mouse.prev==1){
			mouse.released=1;
		}
		mouse.prev = 0;
	}

}

void InitMenu(){
	DrawMenuBG();
	Sprite* spr = LoadSprite("images/buttonvs.png",200,60,1);
	SetButton(&testbutton,0,VersusClick,-1,"hello",spr,spr,spr,spr,400,400,200,60,1,0,100,111,0,0);
	spr = LoadSprite("images/buttoncc.png",200,60,1);
	SetButton(&chrbutton,0,ChrCrClick,-1,"hello",spr,spr,spr,spr,400,500,200,60,1,0,100,111,0,0);
	
}
void VersusClick(int a,...){
	GoToCharSel();
	//GoToVersus();
}
void ChrCrClick(int a,...){
	GoToCharCr();
}

void UpdateMenu(){
	if(mouse.clicked)
    {
		if(MouseInButton(&testbutton))
			testbutton.onClick(0);
		else if(MouseInButton(&chrbutton))
			chrbutton.onClick(0);
    }else{
		DrawPixel(buffer,0,0,0,mouse.x,mouse.y);

	}
}
extern SDL_Surface *textsurf;
extern TTF_Font *font;
void DrawMenus(){
	DrawSprite(testbutton.button,screen,testbutton.box.x,testbutton.box.y,0);
	DrawSprite(chrbutton.button,screen,chrbutton.box.x,chrbutton.box.y,0);
	
	
	SDL_Color bg_color = {0,0,0};
	
	DrawText("This is A Game",400,43);
	
	
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

int MouseInButton(Button *b){
	if((mouse.x > b->box.x)&&(mouse.x < (b->box.x+b->box.w))&&(mouse.y > b->box.y)&&(mouse.y < (b->box.y+b->box.h)))
		return 1;
	return 0;
}
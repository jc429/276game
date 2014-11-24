#ifndef _MENUS_
#define _MENUS_
#include "graphics.h"

typedef struct		/*ripped from project dark*/
{
  Sprite *button;       /*pointer to active button*/
  Sprite *buttons[3];   /*up to 3 different states for button: idle, selected & active*/
  int off;              /*if button is off, it will not be checked against*/
  int color,hcolor;     /*font colors for normal and highlighted*/
  int font;             /*which font to use*/
  int centered;         /*weather to use left justify or centering*/
  int hasfocus;         /*buttons that have the focus are highlighted in some way*/
  int buttonID;         /*when button is pressed, this is the action sent*/
  SDL_Rect box;         /*area that is clickable*/
  char text[80];        /*text to be written on button*/
  int state;            /*current state : idle, selected or clicked*/
  int frame;            /*which frame of the sprite to use*/
  int hotkey;           /*if there is a hotkey for this button, otherwise its -1*/
  int shown;            /*the the button should be drawn or not*/
  int drawtype;         /*0 sprite 1 drawn 2 drawn translucent.  If drawn the box is used for dimensions*/
}Button;

void SetButton(Button *button,int buttonID,int hotkey, char *text,Sprite *sprite,Sprite *sprite1,Sprite *sprite2,Sprite *sprite3,int x,int y,int w,int h,int shown,int frame,int c1, int c2, int font,int centered);


void InitMenu();
void UpdateMenu();
void DrawMenuBG();
void DrawMenus();

#endif
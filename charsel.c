#include "game.h"

void InitCharSel();
void SelectCharacter(int a,...);

extern GameState_T GameState;
extern SDL_Surface *screen, *buffer;
extern Button b1,b2,b3,b4,b5,b6,b7,b8;
extern Character_T c1,c2;			/*the selected characters*/
extern Mouse mouse;

int player;
char str[32];

void GoToCharSel(){
	GameState = C_SELECT;
	player = 1;
	DrawBG("images/menubg3.png");
	InitCharSel();
}

void InitCharSel(){
	Sprite* spr;
	sprintf(str,"Select P%d's Character",player);
	spr = LoadSprite("images/button.png",200,60,1);
	SetButton(&b1,0,SelectCharacter,-1,"Debug Man",spr,spr,spr,spr,250,200,200,60,1,0,100,111,0,0);
	SetButton(&b2,1,SelectCharacter,-1,"Dr. Doom",spr,spr,spr,spr,250,300,200,60,1,0,100,111,0,0);
	SetButton(&b3,2,SelectCharacter,-1,"Waddle Dee",spr,spr,spr,spr,250,400,200,60,1,0,100,111,0,0);
	SetButton(&b4,3,SelectCharacter,-1,"Mega Man",spr,spr,spr,spr,250,500,200,60,1,0,100,111,0,0);
	SetButton(&b5,4,SelectCharacter,-1,"Wizard",spr,spr,spr,spr,600,200,200,60,1,0,100,111,0,0);
	SetButton(&b6,5,SelectCharacter,-1,"hello",spr,spr,spr,spr,600,300,200,60,1,0,100,111,0,0);
	SetButton(&b7,6,SelectCharacter,-1,"hello",spr,spr,spr,spr,600,400,200,60,1,0,100,111,0,0);
	SetButton(&b8,7,SelectCharacter,-1,"hello",spr,spr,spr,spr,600,500,200,60,1,0,100,111,0,0);
	
}

void SelectCharacter(int a,...){
	int f,p;
	va_list args;
	va_start(args,a);
	f = va_arg(args,int);
	p = va_arg(args,int);
	va_end(args);
	if(p==1){
		c1 = (Character_T)f;
		player=2;
		InitCharSel();
	}else if(p==2){
		c2 = (Character_T)f;
		GoToVersus();
	}
}

void DrawCharSel(){
	DrawText(str,400,30);
	DrawButton(&b1);
	DrawButton(&b2);
	DrawButton(&b3);
	DrawButton(&b4);
	DrawButton(&b5);
	DrawButton(&b6);
	DrawButton(&b7);
	DrawButton(&b8);
}

void UpdateCharSel(){
	if(mouse.clicked)
    {
		if(MouseInButton(&b1))
			b1.onClick(1,b1.buttonID,player);
		if(MouseInButton(&b2))
			b2.onClick(1,b2.buttonID,player);
		if(MouseInButton(&b3))
			b3.onClick(1,b3.buttonID,player);
		if(MouseInButton(&b4))
			b4.onClick(1,b4.buttonID,player);
		if(MouseInButton(&b5))
			b5.onClick(1,b5.buttonID,player);
		if(MouseInButton(&b6))
			b6.onClick(1,b6.buttonID,player);
		if(MouseInButton(&b7))
			b7.onClick(1,b7.buttonID,player);
		if(MouseInButton(&b8))
			b8.onClick(1,b8.buttonID,player);
    }
	DrawMouse();
}
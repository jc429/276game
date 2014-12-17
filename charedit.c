#include "game.h"

#define NUMCHARS 8 /*number of characters in the game*/
#define NUMANIMS 5 /*animations per character*/

void CharEdit (int i);
void (*OnClick)(int);
void EditCharacter(int a,...);

Fighter *efighter;
extern GameState_T GameState;
extern SDL_Surface *screen, *buffer;
extern Button b1,b2,b3,b4,b5,b6,b7,b8;
extern Mouse mouse;
int loaded;
Uint8 inputs =  00000000;
Sprite *spr, *plus, *minus;
int f; /*which fighter is loaded?*/

int framedata[NUMCHARS][NUMANIMS] = {  
	/*idle, idle2, walk, run, crouch,jumpsq,jump,fall,land,dashf,dashb, */
	/*For Now: {idle,attack1,attack2,hit,dead}*/
	/*each int denotes the starting point for the animation, in a set order*/
	/*MAKE SURE EVERY INT IS LARGER THAN THE ONE BEFORE IT*/
	/*there needs to be a final int that isn't a seed, for the final animation's length*/
	/*C1 - DEBUG*/{16,32, 33, 34, 35} ,   /*  frames for character 1 */
	/*C2 - DOOM*/{0, 20, 24, 28, 29} ,   /*  ex: column 1 is idle, then idle2, then attacking, then hit, etc */
	/*C3 - WADDLE*/{0, 4, 11, 15, 16},
	/*C4 - MEGAMAN*/{0, 2, 3, 4, 5},
	/*C5 - WIZARD*/{0, 10, 22, 30, 31},
	/*C6*/{0, 1, 2, 3, 4},
	/*C7*/{0, 1, 2, 3, 4},
	/*C8*/{0, 1, 2, 3, 4}
	
};

void GoToCharCr(){
	GameState = C_CREATOR;
	InitCharCrSel();
}

void InitCharCrSel(){
	loaded = 0;
	DrawBG("images/menubg2.png");
	efighter = (Fighter*)malloc(sizeof(Fighter));
	spr = LoadSprite("images/button.png",200,60,1);
	plus = LoadSprite("images/plus.png",32,32,1);
	minus = LoadSprite("images/minus.png",32,32,1);
	SetButton(&b1,0,EditCharacter,-1,"Debug Man",spr,spr,spr,spr,250,200,200,60,1,0,100,111,0,0);
	SetButton(&b2,1,EditCharacter,-1,"Dr. Doom",spr,spr,spr,spr,250,300,200,60,1,0,100,111,0,0);
	SetButton(&b3,2,EditCharacter,-1,"Waddle Dee",spr,spr,spr,spr,250,400,200,60,1,0,100,111,0,0);
	SetButton(&b4,3,EditCharacter,-1,"Mega Man",spr,spr,spr,spr,250,500,200,60,1,0,100,111,0,0);
	SetButton(&b5,4,EditCharacter,-1,"Wizard",spr,spr,spr,spr,600,200,200,60,1,0,100,111,0,0);
	SetButton(&b6,5,EditCharacter,-1,"hello",spr,spr,spr,spr,600,300,200,60,1,0,100,111,0,0);
	SetButton(&b7,6,EditCharacter,-1,"hello",spr,spr,spr,spr,600,400,200,60,1,0,100,111,0,0);
	SetButton(&b8,7,EditCharacter,-1,"hello",spr,spr,spr,spr,600,500,200,60,1,0,100,111,0,0);
}

void InitCharCr(){
	int bx = 250;
	int by = 200;
	SetButton(&b1,0,DecrementStat,-1,"",minus,spr,spr,spr,bx,by,32,32,1,0,100,111,0,0);
	SetButton(&b2,1,IncrementStat,-1,"",plus,spr,spr,spr,bx+40,by,32,32,1,0,100,111,0,0);
	by += 50;
	SetButton(&b3,0,DecrementStat,-1,"",minus,spr,spr,spr,bx,by,32,32,1,0,100,111,0,0);
	SetButton(&b4,1,IncrementStat,-1,"",plus,spr,spr,spr,bx+40,by,32,32,1,0,100,111,0,0);
	
	SetButton(&b5,4,SaveCharacter,-1,"Save",spr,spr,spr,spr,600,200,200,60,1,0,100,111,0,0);
}

void IncrementStat(int a,...){
	int *stat;
	va_list args;
	va_start(args,a);
	stat = va_arg(args,int*);
	++*stat;
	va_end(args);
}

void DecrementStat(int a,...){
	int *stat;
	va_list args;
	va_start(args,a);
	stat = va_arg(args,int*);
	--*stat;
	va_end(args);
}

void EditCharacter(int a,...){
	va_list args;
	va_start(args,a);
	f = va_arg(args,int);
	LoadFighter(efighter,(Character_T)f);
	efighter->inputs = &inputs;
	efighter->x = 100;
	efighter->y = 150;
	//Unload the old buttons*/
	loaded = 1;
	DrawBG("images/menubg3.png");
	va_end(args);
	InitCharCr();
}

void DrawCharCr(){
	if(!loaded){
		DrawButton(&b1);
		DrawButton(&b2);
		DrawButton(&b3);
		DrawButton(&b4);
		DrawButton(&b5);
		DrawButton(&b6);
		DrawButton(&b7);
		DrawButton(&b8);
	}
	else if(loaded){
		char str[30];
		sprintf(str,"Health: %d",efighter->healthmax);
		DrawText(str,50,200);
		sprintf(str,"Jumps: %d",efighter->maxjumps);
		DrawText(str,50,250);
		DrawButton(&b1);
		DrawButton(&b2);
		DrawButton(&b3);
		DrawButton(&b4);
		DrawButton(&b5);
		DrawChar(efighter,screen);
		UpdateFrame(efighter);
	}
	DrawMouse();
}

void UpdateCharCr(){
	if(mouse.clicked)
    {
		if(!loaded){
			if(MouseInButton(&b1))
				b1.onClick(1,b1.buttonID);
			if(MouseInButton(&b2))
				b2.onClick(1,b2.buttonID);
			if(MouseInButton(&b3))
				b3.onClick(1,b3.buttonID);
			if(MouseInButton(&b4))
				b4.onClick(1,b4.buttonID);
			if(MouseInButton(&b5))
				b5.onClick(1,b5.buttonID);

		}
		else{
			if(MouseInButton(&b1))
				b1.onClick(1,&efighter->healthmax);
			if(MouseInButton(&b2))
				b2.onClick(1,&efighter->healthmax);
			if(MouseInButton(&b3))
				b3.onClick(1,&efighter->maxjumps);
			if(MouseInButton(&b4))
				b4.onClick(1,&efighter->maxjumps);
			
			if(MouseInButton(&b5))
				b5.onClick(0);
		}
    }else{
		DrawPixel(buffer,0,0,0,mouse.x,mouse.y);
	}
}

void SaveCharacter(int a,...){
	SaveCFG(efighter,GetCharPath(f));
}


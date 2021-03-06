#include "game.h"

extern SDL_Surface *screen; /*pointer to the screen*/
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Surface *videobuffer;	/*pointer to the actual video surface*/
extern SDL_Rect Camera;		
extern Fighter f1,f2;		/*fighter 1 and fighter 2*/

#define MENUBG "images/menubg.png"
#define HUDBG "images/hudbg.png"
#define NEXTTIME 60
#define NUMROUNDS 3

GameState_T GameState;

Stage_T stage;			/*the selected stage*/
Character_T c1,c2;			/*the selected characters*/
int endgame;				/*end the game?*/
int nexttimer;				/*timer until the next round*/
Uint8 pause;					/*pause flag*/
Uint8 p1input = 00000000;	/*lrudabxy - order of buttons mapped to bits*/
Uint8 p2input = 00000000;
Uint8 p1prev,p2prev;
Sprite* pausescr;			/*the pause screen*/
Sprite* p1vic;				/*p1 victory screen*/
Sprite* p2vic;				/*p2 victory screen*/
Sprite* drawvic;			/*draw screen*/

TTF_Font *font;
SDL_Surface *textsurf;

Uint8* keys;				/*keys pressed*/
int i,j,k,l;				/*iterators*/
int debuginputs;			/*Show key presses?*/
int bsize;					/*size of the icons for each key*/
int pausetimer;				/*prevents pause from being spammed every frame*/
int pausequeue;				/*queues a pause for the next frame*/
int fpress;

int done;					/*is the game done running?*/

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
	
	debuginputs = 1,bsize = 5;

	/*defaults for placeholder reasons*/
	c1 = DEBUG;
	c2 = DEBUG;
	stage = ST_PLATFORM;
	/**/
	pause = 0;
	pausequeue = 0;
	fpress = 0;

	Init_All();
/*	SaveCFG(&f2,"res/test.txt");*/
	do
	{   
		UpdateMouse();
		if(pausequeue==1){
			pausequeue=0;
			GamePause();
		}
		pausetimer--;
		InputControl();
		if(GameState==VERSUS){
			DrawVersus();
			UpdateVersus();
		}else if(GameState==MAIN_MENU){
			DrawMainMenu();
			UpdateMenu();
		}else if(GameState==C_SELECT){
			DrawCharSel();
			UpdateCharSel();
		}else if(GameState==C_CREATOR){
			DrawCharCr();
			UpdateCharCr();
		}
		NextFrame();		
		if(endgame){
			done = 1;
		}
			
		ResetBuffer();
	}while(!done);
	exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
	return 0;
}

void Init_All()
{
	
	done = 0;
	nexttimer = -1;
	Init_Graphics();
	InitFont();
	InitMenu();
	InitMouse();
	atexit(CleanUpAll);
}

void InitVersus(){	
	LoadStage(stage);
	InitFighters(c1,c2);
	DrawBG(HUDBG);
	pausescr = LoadSprite("images/pause.png",1024,768,1);
	p1vic = LoadSprite("images/p1win.png",1024,768,1);
	p2vic = LoadSprite("images/p2win.png",1024,768,1);
	drawvic = LoadSprite("images/draw.png",1024,768,1);
}


void Quit(){
	if(GameState==VERSUS){
		GoToMenu();
	}else{
		if(pause)
			GamePause();
		endgame = 1;
	}
}

void CleanUpAll()
{
  CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

void DrawUpdate(){
	if(GameState==MAIN_MENU){
		
		
	}

	if(GameState==VERSUS){
		
	}
	
	
}
void DrawMainMenu(){
	DrawMenus();
	DrawMouse();
}
void DrawVersus(){
	DrawStage(stage);
	DrawFighters(screen);
	DrawMeters(&f1,&f2);	
	DrawNextRoundTimer(nexttimer);
	int DEBUGSTATE = 1;
	if(DEBUGSTATE){
		char a[20];
		sprintf(a,"P1state: %d",f1.state);
		DrawText(a,270,20);
	}
	if(f2.combo>0){
		char a[20]; 
		sprintf(a,"Combo: %d!",f2.combo);
		DrawText(a,40,120);
	}
	if(f1.combo>0){
		char a[20]; 
		sprintf(a,"Combo: %d!",f1.combo);
		DrawText(a,840,120);
	}
}
void GamePause(){
	if(GameState==VERSUS){
		pausetimer = 10;
		if(pause==0){
			pause = 1;
		}else
			pause = 0;
	}
	else
		GoToVersus();
}
void Die(Fighter* f){
		
	if(nexttimer<0){
		f->health=0;
		ChangeState(f,DEAD);
		f->opponent->victories++;		
		if(f->opponent->victories >= NUMROUNDS){
			
			NextGame();
		}
		else
			NextRound();
	}
}

void NextRound(){
	if(nexttimer<0)
		nexttimer=NEXTTIME;
}

void NextGame(){
	f1.victories = 0;
	f2.victories = 0;
	if(nexttimer<0)
		nexttimer=NEXTTIME;
	GoToCharSel();
}
	
void InputControl(){
	p1prev = p1input;
	p2prev = p2input;
	SDL_PumpEvents();
	p1input = 0;
	p2input = 0;
	keys = SDL_GetKeyState(NULL);
	if(keys[SDLK_SPACE]){
		if(pausetimer<=0)
			GamePause();
	}
	if(keys[SDLK_LALT]&&!fpress){ /*if paused advance one frame*/
		if(!pausequeue&&pause){
			pausequeue=1;
			GamePause();
		}
		fpress = 1;
	}else if(!keys[SDLK_LALT]){
		fpress = 0;
	}
	if(keys[SDLK_ESCAPE])
		Quit();


	
	if(keys[SDLK_a])
		p1input |= 1<<7;
	if(keys[SDLK_d])
		p1input |= 1<<6;
	if(keys[SDLK_w])
		p1input |= 1<<5;
	if(keys[SDLK_s])
		p1input |= 1<<4;
	if(keys[SDLK_z])
		p1input |= 1<<3;
	if(keys[SDLK_x])
		p1input |= 1<<2;
	if(keys[SDLK_c])
		p1input |= 1<<1;
	if(keys[SDLK_v])
		p1input |= 1;


	if(keys[SDLK_LEFT])
		p2input |= 1<<7;
	if(keys[SDLK_RIGHT])
		p2input |= 1<<6;
	if(keys[SDLK_UP])
		p2input |= 1<<5;
	if(keys[SDLK_DOWN])
		p2input |= 1<<4;
	if(keys[SDLK_p])
		p2input |= 1<<3;
	if(keys[SDLK_o])
		p2input |= 1<<2;
	if(keys[SDLK_i])
		p2input |= 1<<1;
	if(keys[SDLK_u])
		p2input |= 1;


	if(debuginputs){
		for(i=0;i<8;i++){
			for(int m=0;m<4;m++){
				if(f1.inputs[m] & 1<<i)
					for(k=0;k<bsize;k++)
						for(l=0;l<bsize;l++)
								DrawPixel(screen,255,0,0,5+k+(bsize+1)*(8-i),(m+1)*(bsize+1)+l); 
				else 
					for(k=0;k<bsize;k++)
						for(l=0;l<bsize;l++)
							DrawPixel(screen,0,180,180,5+k+(bsize+1)*(8-i),(m+1)*(bsize+1)+l); 
			}
		}
		for(i=0;i<8;i++){
			for(int m=0;m<4;m++){
				if(f2.inputs[m] & 1<<i)
					for(k=0;k<bsize;k++)
						for(l=0;l<bsize;l++)
							DrawPixel(screen,255,0,0,955+k+(bsize+1)*(8-i),(m+1)*(bsize+1)+l); 
				else 
					for(k=0;k<bsize;k++)
						for(l=0;l<bsize;l++)
							DrawPixel(screen,0,180,180,955+k+(bsize+1)*(8-i),(m+1)*(bsize+1)+l);
			}
		}
	}

}

void UpdateVersus(){
	if(!pause){	
		UpdateStage(stage);
		if((f2.state!=DEAD)&&(f1.state!=DEAD)){
			if(f1.state!=HIT&&f1.state!=DEAD)
				UpdateFrame(&f1);
			if(f2.state!=HIT&&f2.state!=DEAD)
				UpdateFrame(&f2);
			UpdateInputs(&f1,p1input);
			UpdateInputs(&f2,p2input);
			ProcessInputs(&f1);
			ProcessInputs(&f2);
			FighterThink(&f2);
			FighterThink(&f1);
			FighterUpdate(&f1);
			FighterUpdate(&f2);
			
		}
		if(nexttimer>0){
			if(f2.state==DEAD && f1.state==DEAD)
				DrawSprite(drawvic,screen,0,0,0);
			else if(f2.state==DEAD)
				DrawSprite(p1vic,screen,0,0,0);
			else
				DrawSprite(p2vic,screen,0,0,0);
			nexttimer--;
		}if(nexttimer==0){
			nexttimer--;
			if(stage==ST_PLATFORM)
				stage = ST_FIELD;
			else
				stage = ST_PLATFORM;
			ClearFighter(&f1);
			ClearFighter(&f2);
			InitVersus();
		}
		
	}
	else
		DrawPause(pausescr);
}





void GoToVersus(){
	GameState = VERSUS;	
	InitVersus();
}

void GoToMenu(){
	GameState = MAIN_MENU;
	InitMenu();
}


void GoToStageSel(){
	GameState = S_SELECT;
}

void DrawCharSel();
void DrawStageSel();

void InitFont(){
	font = TTF_OpenFont("fonts/font2.ttf",26);
}
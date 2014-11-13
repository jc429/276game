#include "game.h"

extern SDL_Surface *screen; /*pointer to the screen*/
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;		
extern Fighter f1,f2;		/*fighter 1 and fighter 2*/

#define MENUBG "images/menubg.png"
#define HUDBG "images/hudbg.png"
#define NEXTTIME 60
#define NUMROUNDS 3

StageList stage;			/*the selected stage*/
Character_T c1,c2;			/*the selected characters*/
int endgame;				/*end the game?*/
int nexttimer;				/*timer until the next round*/
Uint8 pause;					/*pause flag*/
Uint8 p1input = 00000000;	/*lrudabxy - order of buttons mapped to bits*/
Uint8 p2input = 00000000;
char* pauseloc;				/*location of the pause screen*/
Sprite* pausescr;			/*the pause screen*/
Sprite* p1vic;				/*p1 victory screen*/
Sprite* p2vic;				/*p2 victory screen*/
Sprite* drawvic;			/*draw screen*/

Uint8* keys;				/*keys pressed*/
int i,j,k,l;				/*iterators*/
int debuginputs;			/*Show key presses?*/
int bsize;					/*size of the icons for each key*/

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
	int done;

	/*defaults for placeholder reasons*/
	c1 = DOOM;
	c2 = WADDLE;
	stage = ST_PLATFORM;
	/**/
	pause = 0;

	Init_All();
	GameState = VERSUS;
	done = 0;
	nexttimer = -1;
	do
	{   
		InputControl();
		if(!pause){		
			Update_All();
			DrawUpdate();
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
			/*	c1 = DOOM;
				c2 = DOOM;*/
				if(stage==ST_PLATFORM)
					stage = ST_FIELD;
				else
					stage = ST_PLATFORM;
				ClearFighter(&f1);
				ClearFighter(&f2);
				InitVersus();
			}
			NextFrame();
			
			if(endgame){
				done = 1;
			}
			
			ResetBuffer();

		}
		else
			DrawPause(pausescr);
	}while(!done);
	exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
	return 0;
}

void Init_All()
{
	Init_Graphics();
	
	InitVersus();
	InitMouse();
	atexit(CleanUpAll);
}

void InitVersus(){	
	LoadStage(stage);
	InitFighters(c1,c2);
	DrawBG(HUDBG);
	pauseloc = "images/pause.png";
	pausescr = LoadSprite(pauseloc,1024,768,1);
	p1vic = LoadSprite("images/p1win.png",1024,768,1);
	p2vic = LoadSprite("images/p2win.png",1024,768,1);
	drawvic = LoadSprite("images/draw.png",1024,768,1);
}

void Update_All()
{
	if(GameState==VERSUS)
	{
		if((f2.state!=DEAD)&&(f1.state!=DEAD)){
			FighterInputs(&f1,p1input);
			FighterInputs(&f2,p2input);
			FighterThink(&f2);
			FighterThink(&f1);
			FighterUpdate(&f1);
			FighterUpdate(&f2);
		}
		UpdateStage();
	}
		
}

void Quit(){
	if(pause)
		GamePause();
	endgame = 1;
}

void CleanUpAll()
{
  CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

void DrawUpdate(){
	if(GameState==MAIN_MENU){
		DrawMenu();
		DrawMouse();
	}

	if(GameState==VERSUS){
		DrawStage(stage);
		DrawFighters(screen);
		DrawMeters(&f1,&f2);	
		DrawNextRoundTimer(nexttimer);
	}
	
	/*int mx,my;
	if(SDL_GetMouseState(&mx,&my))
    {
		DrawSprite(tile,buffer,(mx /32) * 32,(my /32) * 32,0); 
    }*/
}

void GamePause(){
	if(pause==0){
		DrawSprite(pausescr,screen,0,0,1);
		pause = 1;
	}else
		pause = 0;
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
}
	
void InputControl(){
	
	SDL_PumpEvents();
	p1input = 0;
	p2input = 0;
	keys = SDL_GetKeyState(NULL);
	if(keys[SDLK_SPACE])
		GamePause();
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


	debuginputs = 1,bsize = 5;
	if(debuginputs){
		for(i=0;i<8;i++){
			if(p1input & 1<<i)
				for(k=0;k<bsize;k++)
					for(l=0;l<bsize;l++)
						DrawPixel(screen,255,0,0,5+k+(bsize+1)*(8-i),5+l); 
			else 
				for(k=0;k<bsize;k++)
					for(l=0;l<bsize;l++)
						DrawPixel(screen,0,180,180,5+k+(bsize+1)*(8-i),5+l); 
		}
		for(i=0;i<8;i++){
			if(p2input & 1<<i)
				for(k=0;k<bsize;k++)
					for(l=0;l<bsize;l++)
						DrawPixel(screen,255,0,0,5+k+(bsize+1)*(8-i),12+l); 
			else 
				for(k=0;k<bsize;k++)
					for(l=0;l<bsize;l++)
						DrawPixel(screen,0,180,180,5+k+(bsize+1)*(8-i),12+l); 
		}
	}

}

	/*SDL_Event events;
	while(SDL_PollEvent(&events)){
		switch(events.type){
			case SDL_KEYDOWN:
				switch(events.key.keysym.sym){
					case SDLK_SPACE:
						GamePause();
						break;
					case SDLK_a:
						p1input |= 1<<7;
						break;
					case SDLK_d:
						p1input |= 1<<6;
						break;
					case SDLK_w:
						p1input |= 1<<5;
						break;
					case SDLK_s:
						p1input |= 1<<4;
						break;
					case SDLK_z:
						p1input |= 1<<3;
						break;
					case SDLK_x:
						p1input |= 1<<2;
						break;
					case SDLK_c:
						p1input |= 1<<1;
						break;
					case SDLK_v:
						p1input |= 1;
						break;

					case SDLK_LEFT:
						p2input |= 1<<7;
						break;
					case SDLK_RIGHT:
						p2input |= 1<<6;
						break;
					case SDLK_UP:
						p2input |= 1<<5;
						break;
					case SDLK_DOWN:
						p2input |= 1<<4;
						break;
					case SDLK_p:
						p2input |= 1<<3;
						break;
					case SDLK_o:
						p2input |= 1<<2;
						break;
					case SDLK_i:
						p2input |= 1<<1;
						break;
					case SDLK_u:
						p2input |= 1;
						break;


				
				
				}
				continue;
			case SDL_KEYUP:
				switch(events.key.keysym.sym){
					case SDLK_ESCAPE:
						Quit();
						break;
					case SDLK_a:
						p1input &= 0<<7;
						break;
					case SDLK_d:
						p1input &= 0<<6;
						break;
					case SDLK_w:
						p1input &= 0<<5;
						break;
					case SDLK_s:
						p1input &= 0<<4;
						break;
					case SDLK_z:
						p1input &= 0<<3;
						break;
					case SDLK_x:
						p1input &= 0<<2;
						break;
					case SDLK_c:
						p1input &= 0<<1;
						break;
					case SDLK_v:
						p1input &= 0;
						break;
					case SDLK_LEFT:
						p2input &= 0<<7;
						break;
					case SDLK_RIGHT:
						p2input &= 0<<6;
						break;
					case SDLK_UP:
						p2input &= 0<<5;
						break;
					case SDLK_DOWN:
						p2input &= 0<<4;
						break;
					case SDLK_p:
						p2input &= 0<<3;
						break;
					case SDLK_o:
						p2input &= 0<<2;
						break;
					case SDLK_i:
						p2input &= 0<<1;
						break;
					case SDLK_u:
						p2input &= 0;
						break;
				}
			default:
				continue;
			


		}
	}*/

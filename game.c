#include "game.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;
extern Fighter f1,f2;

StageList stage;
Character_T c1,c2;
int endgame;
int nexttimer;
int pause;
Uint8 p1input = 00000000; /*lrudabxy - order of buttons mapped to bits*/
Uint8 p2input = 00000000;

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
	Sprite *tile;
	int done;
	int keyn;

	c1 = DOOM;
	c2 = WADDLE;
	stage = ST_PLATFORM;
	pause = 0;

	Init_All();
	GameState = VERSUS;
	DrawBG("images/bgimage1.png");
	done = 0;
	nexttimer = -1;
	do
	{   
		FighterControl();
		if(!pause){		
			Update_All();
			DrawUpdate();
			if(nexttimer>0)
				nexttimer--;
			if(nexttimer==0){
				c1 = DOOM;
				c2 = DOOM;
				stage = ST_FIELD;
				InitVersus();
			}
			NextFrame();
			SDL_PumpEvents();
			if(endgame){
				done = 1;
			}
			
			ResetBuffer();
		}
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
}

void Update_All()
{
	if(GameState==VERSUS)
	{
		FighterInputs(&f1,p1input);
		FighterInputs(&f2,p2input);
		FighterThink(&f1);
		FighterThink(&f2);
		FighterUpdate(&f1);
		FighterUpdate(&f2);
		UpdateStage();
	}
		
}

void Quit(){
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
		DrawNextRoundTimer();
	}
	
	/*int mx,my;
	if(SDL_GetMouseState(&mx,&my))
    {
		DrawSprite(tile,buffer,(mx /32) * 32,(my /32) * 32,0); 
    }*/
}

void GamePause(){
	if(pause==0)
		pause = 1;
	else
		pause=0;
}

void NextRound(){
	nexttimer=52;
}

void DrawNextRoundTimer(){
	for(int i=0;i<nexttimer;i++){
		DrawPixel(screen,200,0,0,i,20); 
		DrawPixel(screen,200,0,0,i,21); 
		DrawPixel(screen,200,0,0,i,22); 
	}

}
void FighterControl(){
	

	SDL_Event events;
//	Uint8* keys = SDL_GetKeyState(NULL);
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
				case SDLK_BACKSLASH:
					p2input |= 1<<3;
					break;
				case SDLK_7:
					p2input |= 1<<2;
					break;
				case SDLK_8:
					p2input |= 1<<1;
					break;
				case SDLK_9:
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
				case SDLK_BACKSLASH:
					p2input &= 0<<3;
					break;
				case SDLK_7:
					p2input &= 0<<2;
					break;
				case SDLK_8:
					p2input &= 0<<1;
					break;
				case SDLK_9:
					p2input &= 0;
					break;
				}
			default:
				continue;
			


		}
	}
	/*
	if(keys[SDLK_a])
		p1input |= 1<<7;
	if(keys[SDLK_d])
		p1input |= 1<<6;
	if(keys[SDLK_w])
		p1input |= 1<<5;
	if(keys[SDLK_s])
		p1input |= 1<<4;


	if(keys[SDLK_LEFT])
		p2input |= 1<<7;
	if(keys[SDLK_RIGHT])
		p2input |= 1<<6;
	if(keys[SDLK_UP])
		p2input |= 1<<5;
	if(keys[SDLK_DOWN])
		p2input |= 1<<4;
	*/

	int debuginputs = 1,bsize = 5;
	if(debuginputs){
		for(int i=0;i<8;i++){
			if(p1input & 1<<i)
				for(int k=0;k<bsize;k++)
					for(int l=0;l<bsize;l++)
						DrawPixel(screen,255,0,0,5+k+(bsize+1)*i,5+l); 
			else 
				for(int k=0;k<bsize;k++)
					for(int l=0;l<bsize;l++)
						DrawPixel(screen,0,0,255,5+k+(bsize+1)*i,5+l); 
		}
		for(int i=0;i<8;i++){
			if(p2input & 1<<i)
				for(int k=0;k<bsize;k++)
					for(int l=0;l<bsize;l++)
						DrawPixel(screen,255,0,0,5+k+(bsize+1)*i,12+l); 
			else 
				for(int k=0;k<bsize;k++)
					for(int l=0;l<bsize;l++)
						DrawPixel(screen,0,0,255,5+k+(bsize+1)*i,12+l); 
		}
	}

}
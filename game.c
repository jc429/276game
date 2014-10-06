#include "game.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;
extern Fighter f1,f2;

void Init_All();
void CleanUpAll();
void DrawUpdate();

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
	Sprite *tile;
	int done;
	int keyn;

	
	
	Uint8 *keys;
	Init_All();
	LoadLevel();
	DrawStageBG();
	GameState = VERSUS;
	done = 0;
	do
	{   
		ResetBuffer ();
		FighterControl(keys);
		Update_All();
		DrawUpdate();
		NextFrame();
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&keyn);
    
		if(keys[SDLK_ESCAPE]){
			done = 1;
		}
	}while(!done);
	exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
	return 0;
}

void Init_All()
{
	Init_Graphics();
	InitFighters();
	InitMouse();
	atexit(CleanUpAll);
}

void Update_All()
{
	if(GameState==VERSUS)
	{
		FighterThink(&f1);
		FighterThink(&f2);
		FighterUpdate(&f1);
		FighterUpdate(&f2);
		UpdateStage();
	}
}

void CleanUpAll()
{
  CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

void DrawUpdate(){
	if(GameState==VERSUS){
		DrawStage();
	//	DrawHurtboxes(screen);
	//	DrawHitboxes(screen);
		DrawFighters(screen);
		DrawMeters(&f1,&f2);
		DrawMouse();
	
	}
	/*int mx,my;
	if(SDL_GetMouseState(&mx,&my))
    {
		DrawSprite(tile,buffer,(mx /32) * 32,(my /32) * 32,0); 
    }*/
}

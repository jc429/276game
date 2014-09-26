#include "fighter.h"

Fighter f1, f2;

void MakeFighter(Fighter* f){
	
}
void LoadFighter(Fighter* f, Character_T c){
//	char* temp;			//temporary thing we read from file
	char* filepath;

	/////Switch depending on character selected/////
	if(c==DOOM)
		filepath = "res/chr/doom.txt";
	else
		filepath = "res/chr/doom.txt";
	///////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////this is a mess 
	{ ///file loader/////
		
		char buf[255];
		int cur_line = 0;
		FILE *fileptr = NULL;
		fileptr = fopen(filepath,"r");
		if(!fileptr){
			fprintf(stderr,"couldn't find character file: ",filepath);
			return;
		}

		
		if(fscanf(fileptr,"%s",buf)){
			if(strcmp(buf,"tile_width:")==0){
				fscanf(fileptr,"%i",&f->t_width);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"tile_height:")==0){
				fscanf(fileptr,"%i",&f->t_height);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"tiles_per_row:")==0){
				fscanf(fileptr,"%i",&f->t_per_row);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"sprite:")==0){
				fscanf(fileptr,"%s",buf);
				//spritepath = buf;		
				f->f_sprite = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_sprite == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load sprite\n");
				fscanf(fileptr,"%s",buf);
			}	
			if(strcmp(buf,"hitbox:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_hitbox = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_hitbox == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load hitboxes\n");
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"hurtbox:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_hurtbox = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_hurtbox == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load hurtboxes\n");
			}
		}
		fclose(fileptr);

		
		
		
	}

	
	
	
/////////////////////////////////////////////////////////////////////////	


	
	

	f->x_off = 90;
	f->y_off = 130;

	f->x = 30;
	f->y = 350;


}

void FighterThink(Fighter *f){

}


void FighterUpdate(Fighter *f){
	f->x += 4;
}

void ClearFighter(Fighter *f){
	if(f->f_sprite!= NULL)FreeSprite(f->f_sprite);
	if(f->f_hitbox!= NULL)FreeSprite(f->f_hitbox);
	if(f->f_hurtbox!= NULL)FreeSprite(f->f_hurtbox);
}


void DrawFighters(SDL_Surface* screen)
{
	
	bool showpoints = true;
	//f1.y = 300; f1.x = 200;
	f2.y = 300; f2.x = 300;
	
	if(f1.f_sprite != NULL) {
		DrawChar(&f1,f1.f_sprite,screen);
	}
	if(f2.f_sprite != NULL) {
		DrawChar(&f2,f2.f_hitbox,screen);
	}
	if(showpoints){
		DrawPlayerPoint(&f1);
		DrawPlayerPoint(&f2);
		
	}
}

void DrawChar(Fighter* f, Sprite* spr, SDL_Surface* screen){

	DrawSprite(spr,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
	f->frame = ((f->frame + 1)%10);
}

void InitFighters()
{
	LoadFighter(&f1,DOOM);
	LoadFighter(&f2,NOTDOOM);
	//f1.fsprite = LoadSprite("images/idoom.png",180, 150, 10);
	
}


void FighterControl(Uint8* keys){
	int keyn;
	keys = SDL_GetKeyState(&keyn);

	if(keys[SDLK_f])
		f1.x += 5;

}

void Update_All()
{
	FighterThink(&f1);
	FighterThink(&f2);
	FighterUpdate(&f1);
	FighterUpdate(&f2);
	
}
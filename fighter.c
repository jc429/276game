#include "fighter.h"

#define FLOOR 550


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

		f->anim_length = 10;
		f->anim_seed = 0;
		f->frame = 0;
		f->grounded = true;
		
	}

	
	
	
/////////////////////////////////////////////////////////////////////////	


	
	

	f->x_off = 90;
	f->y_off = 130;

	f->y = FLOOR;

	f1.x = 200;
	f2.x = 500;
}

void FighterThink(Fighter *f){

}


void FighterUpdate(Fighter *f){
	
	if(!f->grounded){
		if(f->y+f->vy > FLOOR){
			f->y = FLOOR;
			f->vy = 0;
			f->grounded = true;
		}
		else{
			f->y += f->vy;
			f->vy+=2; 
		}
	}

	if(f->x < 1) f->x = 1;
	if(f->x > 1024) f->x = 1024;
	if(f->y < 1){ f->y = 1; f->vy = 0;}
}

void ClearFighter(Fighter *f){
	if(f->f_sprite!= NULL)FreeSprite(f->f_sprite);
	if(f->f_hitbox!= NULL)FreeSprite(f->f_hitbox);
	if(f->f_hurtbox!= NULL)FreeSprite(f->f_hurtbox);
}


void DrawFighters(SDL_Surface* screen)
{
	bool showsprites = true;
	bool showpoints = true;
	bool showhitboxes = true;
	bool showhurtboxes = false;
	
	if(showpoints){
		DrawPlayerPoint(&f1);
		DrawPlayerPoint(&f2);	
	}
	
	if(showhurtboxes){
		if(f1.f_hurtbox != NULL) {
			DrawChar(&f1,f1.f_hurtbox,screen);
		}
		if(f2.f_hurtbox != NULL) {
			DrawChar(&f2,f2.f_hurtbox,screen);
		}
	}
	if(showhitboxes){
		if(f1.f_hitbox != NULL) {
			DrawChar(&f1,f1.f_hitbox,screen);
		}
		if(f2.f_hitbox != NULL) {
			DrawChar(&f2,f2.f_hitbox,screen);
		}
	}
	if(showsprites){
		if(f1.f_sprite != NULL) {
			DrawChar(&f1,f1.f_sprite,screen);
		}
		if(f2.f_sprite != NULL) {
			DrawChar(&f2,f2.f_sprite,screen);
		}
	}
	UpdateFrame(&f1);
	UpdateFrame(&f2);
}

void DrawChar(Fighter* f, Sprite* spr, SDL_Surface* screen){
	DrawSprite(spr,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
}
void UpdateFrame(Fighter* f){
	f->frame = (((f->frame + 1)%(f->anim_length))+f->anim_seed);
	if(f->frame+1 >= (f->anim_length+f->anim_seed))
		ChangeState(f,IDLE);
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

	if(keys[SDLK_d])
		f1.x += 5;
	if(keys[SDLK_a])
		f1.x-=5;
	if(keys[SDLK_w])
		Jump(&f1);

	if(keys[SDLK_LEFT])
		f2.x -= 5;
	if(keys[SDLK_RIGHT])
		f2.x += 5;
	if(keys[SDLK_UP])
		Jump(&f2);


	if(keys[SDLK_x]){
		ChangeState(&f1,ATK_N_P);
	}
	else if(keys[SDLK_q])
		ChangeState(&f1,IDLE);
}

void Update_All()
{
	FighterThink(&f1);
	FighterThink(&f2);
	FighterUpdate(&f1);
	FighterUpdate(&f2);
	
}

void ChangeState(Fighter* f, State_T st){
	if(f->state != st){
		f->state = st;
		if(st == ATK_N_P){
			f->anim_seed = 20;
			f->anim_length = 4;
		}
		if(st==IDLE){
			f->anim_length = 10;
			f->anim_seed = 0;
		
		}
		f->frame = f->anim_seed;
	}

}

void Jump(Fighter* f){
	if(f->grounded){
		f->state = JUMP_G_N;
		f->vy = -20;
		f->grounded = false;
	}
}


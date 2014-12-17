#include "fighter.h"
#include "stage.h"

#define NUMCHARS 3 /*number of characters in the game*/
#define NUMANIMS 5 /*animations per character*/
/*** Where should this goooooo? ***/
extern int framedata[NUMCHARS][NUMANIMS];
extern Uint8 p1input, p2input;


//extern int STAGEFLOOR,STAGELEFT,STAGERIGHT,P1SPAWN,P2SPAWN;
extern SDL_Surface *screen; /*pointer to the screen*/
Fighter f1, f2;
extern Stage st;

char *names[8] = {"debug","doom","waddle","mega","wiz"};


/**************************************************************************************************/

/** Initializes both fighters for battle*/
void InitFighters(Character_T p1, Character_T p2)
{
	LoadFighter(&f1,p1);
	LoadFighter(&f2,p2);
	f1.opponent = &f2;
	f2.opponent = &f1;	
	f1.inputs = &p1input;
	f1.controls = 1;
	f1.y = st.platform_list->p_ypos;
	f2.inputs = &p2input;
	f2.controls = 1;
	f2.y = st.platform_list->p_ypos;
	f1.x = st.P1spawn;
	f2.x = st.P2spawn;
	f1.facing = 1;
	f2.facing = -1;
}

/** Clears a fighter for reuse*/
void ClearFighter(Fighter *f){
	if(f->f_spritel!= NULL)FreeSprite(f->f_spritel);
	if(f->f_spriter!= NULL)FreeSprite(f->f_spriter);
	if(f->f_hitboxl!= NULL)FreeSprite(f->f_hitboxl);
	if(f->f_hitboxr!= NULL)FreeSprite(f->f_hitboxr);
	if(f->f_hurtboxl!= NULL)FreeSprite(f->f_hurtboxl);
	if(f->f_hurtboxr!= NULL)FreeSprite(f->f_hurtboxr);
}
/**************************************************************************************************/

/** Draws the fighters to the screen*/
void DrawFighters(SDL_Surface* surf)
{
	int showsprites = 1;	/**< Draw the sprites? */
	int showpoints = 1;		/**< Show their points?*/
	/******************************/
	if(showsprites){
		if((f1.f_spriter != NULL)&&((f1.f_spriter != NULL))) {
			DrawChar(&f1,surf);
		}
		if((f2.f_spriter != NULL)&&((f2.f_spritel != NULL))){
			DrawChar(&f2,surf);
		}
	}
	/******************************/
	if(showpoints){
		DrawPlayerPoint(&f1);
		DrawPlayerPoint(&f2);	
	}	

	
}

void DrawChar(Fighter* f, SDL_Surface* screen){
	int drawsprite = 1;
	int drawhitbox = 0;
	int drawhurtbox = 0;

	if(f->state!=ATTACKING)
		CheckFacing(f);
	
	if(drawhurtbox)
		DrawSprite(f->f_hurtbox,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
	
	if(drawsprite)
		DrawSprite(f->f_sprite,screen,f->x-f->x_off,f->y-f->y_off,f->frame);

	if(drawhitbox)
		DrawSprite(f->f_hitbox,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
}

/** check which way the fighter is facing and update sprites accordingly*/
void CheckFacing(Fighter* f){
	if (f->facing == -1){
		f->f_sprite = f->f_spritel;
		f->f_hitbox = f->f_hitboxl;
		f->f_hurtbox = f->f_hurtboxl;
	}
	else if(f->facing == 1){
		f->f_sprite = f->f_spriter;
		f->f_hitbox = f->f_hitboxr;
		f->f_hurtbox = f->f_hurtboxr;
	}
}

/** update the fighter's animation frame*/
void UpdateFrame(Fighter* f){	
	f->frame += 1;
	if(f->frame >= (f->anim_length+f->anim_seed))
		if(*f->inputs == 00000000)
			ChangeState(f,IDLE);

	if(f->frame >= (f->anim_length+f->anim_seed)){
		f->frame = ((f->frame-f->anim_seed)%f->anim_length)+f->anim_seed;
	}
	
	
}
/**************************************************************************************************/
/** load the fighter's data from a config file*/
void LoadFighter(Fighter* f, Character_T c){
	char* filepath;
	f->chr = c;
	filepath = GetCharPath(c);
	f->name = names[c];

	if(LoadCFG(f,filepath)){ /*file loader*/
	

		/*DEBUG STUFF CHANGE LATER*/
		f->walkspeed=5;
		/**************************/
	
		f->anim_seed = framedata[f->chr][IDLE]; /*seed is where the animation begins on the sheet*/
		f->anim_length = framedata[f->chr][IDLE+1]-framedata[f->chr][IDLE]; /*anim length is just the distance to the next seed*/	
		f->frame = f->anim_seed;
		f->state=IDLE;
		f->hasjump = f->maxjumps;
		f->grounded = 1;
		f->health = f->healthmax;
		f->hitstun = 0;
		
		f->f_sprite = f->f_spritel;
		f->f_hitbox = f->f_hitboxl;
		f->f_hurtbox = f->f_hurtboxl;
	}else{
		fprintf(stderr,"load failed");
	}

}

char* GetCharPath(int c){
	switch(c){
		case DOOM:
			return "res/chr/doom.txt";
		case WADDLE:
			return "res/chr/waddle.txt";
		case C4:
			return "res/chr/mega.txt";
		case C5:
			return "res/chr/wiz.txt";
		case C6:
		case C7:
		case C8:	
		case DEBUG:
			return "res/chr/debug.txt";
	}
}

int LoadCFG(Fighter* f,char* path){
		
		char buf[255];
		int cur_line = 0;
		FILE *fileptr = NULL;
		fileptr = fopen(path,"r");
		if(!fileptr){
			fprintf(stderr,"couldn't find character file: ",path);
			return 0;
		}

		
		if(fscanf(fileptr,"%s",buf)){
			if(strcmp(buf,"tile_width:")==0){ 
				fscanf(fileptr,"%i",&f->t_width);
				fscanf(fileptr,"%s",buf);}
			if(strcmp(buf,"tile_height:")==0){
				fscanf(fileptr,"%i",&f->t_height);
				fscanf(fileptr,"%s",buf);}
			if(strcmp(buf,"tiles_per_row:")==0){
				fscanf(fileptr,"%i",&f->t_per_row);
				fscanf(fileptr,"%s",buf);}
			if(strcmp(buf,"x_offset:")==0){
				fscanf(fileptr,"%i",&f->x_off);
				fscanf(fileptr,"%s",buf);}
			if(strcmp(buf,"y_offset:")==0){
				fscanf(fileptr,"%i",&f->y_off);
				fscanf(fileptr,"%s",buf);}

			if(strcmp(buf,"sprite_l:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_spritel = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_spritel == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load sprite\n");
				fscanf(fileptr,"%s",buf);
			}if(strcmp(buf,"sprite_r:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_spriter = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_spriter == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load sprite\n");
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"hitbox_l:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_hitboxl = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_hitboxl == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load hitboxes\n");
				fscanf(fileptr,"%s",buf);
			}if(strcmp(buf,"hitbox_r:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_hitboxr = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_hitboxr == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load hitboxes\n");
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"hurtbox_l:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_hurtboxl = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_hurtboxl == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load hurtboxes\n");
				fscanf(fileptr,"%s",buf);
			}if(strcmp(buf,"hurtbox_r:")==0){
				fscanf(fileptr,"%s",buf);
				f->f_hurtboxr = LoadSprite(buf,f->t_width, f->t_height, f->t_per_row);
				if(f->f_hurtboxr == NULL)fprintf(stdout,"FATAL ERROR: Couldn't load hurtboxes\n");
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"healthmax:")==0){
				fscanf(fileptr,"%i",&f->healthmax);
				fscanf(fileptr,"%s",buf);
			}
			if(strcmp(buf,"maxjumps:")==0){
				fscanf(fileptr,"%i",&f->maxjumps);
				fscanf(fileptr,"%s",buf);
			}
		}
		fclose(fileptr);
		return 1;
}

int SaveCFG(Fighter* f,char* path){
	char buf[255];
		int cur_line = 0;
		FILE *fileptr = NULL;
		fileptr = fopen(path,"w");
		if(!fileptr){
			fprintf(stderr,"couldn't find character file: ",path);
			return 0;
		}
		fprintf(fileptr,"tile_width: %d\n",f->t_width);
		fprintf(fileptr,"tile_height: %d\n",f->t_height);
		fprintf(fileptr,"tiles_per_row: %d\n",f->t_per_row);
		fprintf(fileptr,"x_offset: %d\n",f->x_off);
		fprintf(fileptr,"y_offset: %d\n",f->y_off);
		
		fprintf(fileptr,"sprite_l: res/chr/%s/sprite_l.png \n",f->name);
		fprintf(fileptr,"sprite_r: res/chr/%s/sprite_r.png \n",f->name);
		fprintf(fileptr,"hitbox_l: res/chr/%s/hitbox_l.png \n",f->name);
		fprintf(fileptr,"hitbox_r: res/chr/%s/hitbox_r.png \n",f->name);
		fprintf(fileptr,"hurtbox_l: res/chr/%s/hurtbox_l.png \n",f->name);
		fprintf(fileptr,"hurtbox_r: res/chr/%s/hurtbox_r.png \n",f->name);
		
		fprintf(fileptr,"healthmax: %d\n",f->healthmax);
		fprintf(fileptr,"maxjumps: %d\n",f->maxjumps);
	

		fclose(fileptr);
		return 1;
}
/**************************************************************************************************/

/** do things based on the player inputs*/
void FighterInputs(Fighter* f,Uint8 inputs){
	if(f->controls){ /*if our controls aren't locked*/
		/*lrudabxy - order of buttons mapped to bits*/
		if((inputs & 1<<7)&&(~inputs & 1<<6)){
			f->facing = -1;
			f->vx = -1*f->walkspeed;
		}
		else if((inputs & 1<<6)&&(~inputs & 1<<7)){
			f->facing = 1;
			f->vx = f->walkspeed;
		}
		else{
			f->vx = 0;
		}
		if(inputs & 1<<5){
			Jump(f);
		}
		if(inputs & 1<<3){
			Attack(f,ATK_N_P);
		}
		if(inputs & 1<<2){
			Attack(f,ATK_N_K);
		}
	}
}

/** for animations and state changing stuff*/
void FighterThink(Fighter *f){ 
	int alwaysFaceOpponent = 0; /**< maybe we want to always be facing the opponent like street fighter*/
	if(f->health<=0&&f->state!=DEAD)
		Die(f);
	else{
		
		if(f->state!=HIT&&f->hitstun>0)
			ChangeState(f,HIT);
		if(f->state==ATTACKING)
			CollisionCheck(f);
		if(alwaysFaceOpponent){
			if(f->x < f->opponent->x)
				f->facing = 1;
			else if(f->x > f->opponent->x)
				f->facing = -1;
			CheckFacing(f);
		}
		if(f->jumptimer>=0)
			f->jumptimer--;

		if(f->hitstun>=0){
			if(f->hitstun==0){
				f->combo = 0;
			}
			f->hitstun--;
		}
		else if(f->state==HIT)
			ChangeState(f,IDLE);
	}
}

/** for movement and physics stuff*/
void FighterUpdate(Fighter *f){ 
	
	if(CollidePlatform(f,st.platform_list)){
		
		
	}else{
		f->y += f->vy;
		f->vy+=2; 
		if(f->grounded&&f->jumptimer<=0){
			f->grounded=0;
			f->hasjump--;
		}
	}
	

	f->x+=f->vx;

	/*screen collision checks*/
	if(f->x < 1) f->x = 1;
	if(f->x > 1024) f->x = 1024;
	if(f->y < 1){ f->y = 1; f->vy = 0;}
	if(f->y+f->vy > 767){ 
		f->y = 767; 
		f->vy = 0;
		f->grounded=1;
		if(f->state!=DEAD)
			Die(f);
	}
}


/**************************************************************************************************/




/** for changing the fighter's state*/
void ChangeState(Fighter* f, State_T st){
	if(f->state != st){
		f->state = st;
		if(st != ATTACKING){
			f->anim_seed = framedata[f->chr][st]; /*seed is where the animation begins on the sheet*/
			f->anim_length = framedata[f->chr][st+1]-framedata[f->chr][st]; /*anim length is just the distance to the next seed*/
		}else{
			f->anim_seed = framedata[f->chr][st+f->atktype]; 
			f->anim_length = framedata[f->chr][st+f->atktype+1]-framedata[f->chr][st+f->atktype]; 
		}
		f->frame = f->anim_seed;

	}

}

/** apply a jump*/
void Jump(Fighter* f){
	if(f->state!=DEAD&&f->hitstun<0&&f->hasjump>0&&f->jumptimer<0){
	/*	ChangeState(f,JUMP_G_N);*/
		f->vy = -20;
		f->grounded = 0;
		f->hasjump--;
		f->jumptimer = 8;
	}
}

/** apply an attack*/
void Attack(Fighter* f,Attack_T atk){
	f->atktype = atk;
	ChangeState(f,ATTACKING);

}

/** get damaged*/
void TakeHit(Fighter* f, int dmg, int kback, int stun){
	float minscale = 0.25;
	float hitscale = 0.85;
	float scaling = 1;
	for(int i = 0; i < f->combo; i++) /* ghetto exponent math let's go*/
		scaling*=hitscale;
	if(scaling < minscale)
		scaling = minscale;
	f->health -= dmg*scaling;
	f->hitstun = stun;
	f->combo++;
}



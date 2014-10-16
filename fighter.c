#include "fighter.h"

#define NUMCHARS 3 /*number of characters*/
#define NUMANIMS 4 /*animations per character*/

extern int STAGEFLOOR,STAGELEFT,STAGERIGHT,P1SPAWN,P2SPAWN;
extern SDL_Surface *screen; /*pointer to the screen*/
Fighter f1, f2;


/*** Where should this goooooo? ***/
int framedata[NUMCHARS][NUMANIMS] = {  
	/*idle, idle2, walk, run, crouch,jumpsq,jump,fall,land,dashf,dashb, */
	/*For Now: {idle,attack,hit,dead}*/
	/*each int denotes the starting point for the animation, in a set order*/
	/*MAKE SURE EVERY INT IS LARGER THAN THE ONE BEFORE IT*/
	/*DEBUG*/{0, 1, 2, 3} ,   /*  frames for character 1 */
	/*DOOM*/{0, 20, 24, 25} ,   /*  ex: column 1 is idle, then idle2, then attacking, then hit, etc */
	/*WADDLE*/{0, 4, 10, 13}  
};

/**************************************************************************************************/
void InitFighters(Character_T p1, Character_T p2)
{
	LoadFighter(&f1,p1);
	LoadFighter(&f2,p2);
	f1.opponent = &f2;
	f2.opponent = &f1;	
}
void ClearFighter(Fighter *f){
	if(f->f_spritel!= NULL)FreeSprite(f->f_spritel);
	if(f->f_spriter!= NULL)FreeSprite(f->f_spriter);
	if(f->f_hitboxl!= NULL)FreeSprite(f->f_hitboxl);
	if(f->f_hitboxr!= NULL)FreeSprite(f->f_hitboxr);
	if(f->f_hurtboxl!= NULL)FreeSprite(f->f_hurtboxl);
	if(f->f_hurtboxr!= NULL)FreeSprite(f->f_hurtboxr);
}
/**************************************************************************************************/
void DrawFighters(SDL_Surface* surf)
{
	int showsprites = 1;
	int showpoints = 1;
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
	if(f1.state!=HIT)
		UpdateFrame(&f1);
	if(f2.state!=HIT)
		UpdateFrame(&f2);
}

void DrawChar(Fighter* f, SDL_Surface* screen){
	if(f->state!=ATTACKING)
		CheckFacing(f);
	DrawSprite(f->f_sprite,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
	if(0)
		DrawSprite(f->f_hurtbox,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
	if(0)
		DrawSprite(f->f_hitbox,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
}

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

void UpdateFrame(Fighter* f){	
	
	f->frame = (((f->frame + 1)%(f->anim_length))+f->anim_seed);
	if(f->frame+1 >= (f->anim_length+f->anim_seed))
		ChangeState(f,IDLE);
}
/**************************************************************************************************/
void LoadFighter(Fighter* f, Character_T c){
	char* filepath;
	f->chr = c;
	/*//////////////////////////////////////////////*/
	if(c==DOOM)
		filepath = "res/chr/doom.txt";
	else if(c==WADDLE)
		filepath = "res/chr/waddle.txt";
	else
		filepath = "res/chr/nodoom.txt";
	/*//////////////////////////////////////////////*/
	{ /*file loader*/
		
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

		}
		fclose(fileptr);


		/*DEBUG STUFF CHANGE LATER*/
		f->walkspeed=5;
		/**************************/
	
		f->anim_length = 1;
		f->anim_seed = 0;
		f->frame = 0;
		
	//	ChangeState(f,ATTACKING);
		ChangeState(f,IDLE);
		f->grounded = 1;
		f->health = f->healthmax;
		

	}

	f->f_sprite = f->f_spritel;
	f->f_hitbox = f->f_hitboxl;
	f->f_hurtbox = f->f_hurtboxl;

	f->controls = 1;

	f->y = STAGEFLOOR;
	f1.x = P1SPAWN;
	f2.x = P2SPAWN;
	f1.facing = 1;
	f2.facing = -1;

}
/**************************************************************************************************/


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
void FighterThink(Fighter *f){ /*for animations and state changing stuff*/
	if(f->health<=0&&f->state!=DEAD)
		Die(f);
	else{
		
		if(f->state!=HIT&&f->hitstun>0)
			ChangeState(f,HIT);
		if(f->state==ATTACKING)
			CollisionCheck(f);
		/*
		if(f->x < f->opponent->x)
			f->facing = 1;
		else if(f->x > f->opponent->x)
			f->facing = -1;
			*/
		if(f->hitstun>=0)
			f->hitstun--;
		else if(f->state==HIT)
			ChangeState(f,IDLE);
	}
}

void FighterUpdate(Fighter *f){ /* for movement and physics stuff*/
	
	if((f->y+f->vy > STAGEFLOOR)&&(f->x > STAGELEFT)&&(f->x < STAGERIGHT)/*if we would be past the floor this frame*/
	&&((f->y)-STAGEFLOOR < 5)) /*if we're coming from above or really close from below*/
	{
		f->y = STAGEFLOOR;
		f->vy = 0;
		f->grounded = 1;
			
	}else{
		f->y += f->vy;
		f->vy+=2; 
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






void ChangeState(Fighter* f, State_T st){
	if(f->state != st){
		f->state = st;
		
		f->anim_seed = framedata[f->chr][st]; /*seed is where the animation begins on the sheet*/
		f->anim_length = 1+framedata[f->chr][st+1]-framedata[f->chr][st]; /*anim length is just the distance to the next seed*/
		
		f->frame = f->anim_seed;
	}

}

void Jump(Fighter* f){
	if(f->grounded&&f->state!=DEAD&&f->hitstun<0){
		f->state = JUMP_G_N;
		f->vy = -20;
		f->grounded = 0;
	}
}

void Attack(Fighter* f,Attack_T atk){
	ChangeState(f,ATTACKING);
	f->atktype = atk;

}

void TakeHit(Fighter* f, int dmg, int kback, int stun){
	f->health -= dmg;
	f->hitstun = stun;
//	f->y-=2;
}

void Die(Fighter* f){
	
	f->health=0;
	ChangeState(f,DEAD);
	NextRound();
}


#include "fighter.h"


extern int STAGEFLOOR,STAGELEFT,STAGERIGHT,P1SPAWN,P2SPAWN;
extern SDL_Surface *screen; /*pointer to the screen*/
Fighter f1, f2;
Uint8 p1input = 00000000; /*lrudabxy - order of buttons mapped to bits*/
Uint8 p2input = 00000000;

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
	if(f->f_hitbox!= NULL)FreeSprite(f->f_hitbox);
	if(f->f_hurtbox!= NULL)FreeSprite(f->f_hurtbox);
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
}
/*
void DrawHitboxes(SDL_Surface* surf){	
	int showhitboxes = 1;
	if(showhitboxes){
		if(f1.f_hitbox != NULL) {
			DrawChar(&f1,surf);
		}
		if(f2.f_hitbox != NULL) {
			DrawChar(&f2,surf);
		}
	}
}
void DrawHurtboxes(SDL_Surface* surf){
	int showhurtboxes = 1;
	if(showhurtboxes){
		if(f1.f_hurtbox != NULL) {
			DrawChar(&f1,surf);
		}
		if(f2.f_hurtbox != NULL) {
			DrawChar(&f2,surf);
		}
	}
}
*/
void DrawChar(Fighter* f, SDL_Surface* screen){
	if(f->state!=ATTACKING)
		CheckFacing(f);
	DrawSprite(f->f_sprite,screen,f->x-f->x_off,f->y-f->y_off,f->frame);
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

	/*//////////////////////////////////////////////*/
	if(c==DOOM)
		filepath = "res/chr/doom.txt";
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

		f->anim_length = 10;
		f->anim_seed = 0;
		f->frame = 0;
		f->grounded = 1;
		f->health = f->healthmax;
		

	}

	f->f_sprite = f->f_spritel;
		f->f_hitbox = f->f_hitboxl;
		f->f_hurtbox = f->f_hurtboxl;

	f->y = STAGEFLOOR;
	f1.x = P1SPAWN;
	f2.x = P2SPAWN;
}
/**************************************************************************************************/
void FighterControl(){
	

	SDL_Event events;
//	Uint8* keys = SDL_GetKeyState(NULL);
	while(SDL_PollEvent(&events)){
		switch(events.type){
			case SDL_KEYDOWN:
				switch(events.key.keysym.sym){
				case SDLK_ESCAPE:
					Quit();
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

	FighterInputs(&f1,p1input);
	FighterInputs(&f2,p2input);

}

void FighterInputs(Fighter* f,Uint8 inputs){
	
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
void FighterThink(Fighter *f){ /*for animations and state changing stuff*/
	if(f->health<=0)
		Die(f);
	else{
		if(f->state!=HIT&&f->hitstun>0)
			ChangeState(f,HIT);
		if(f->state!=HIT)
			UpdateFrame(f);
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
		switch(st)
		{ /*fill with values from a config file later*/
			case HIT:
				f->anim_seed = 24;
				f->anim_length = 1;
				break;
			case IDLE:
				f->anim_seed = 0;
				f->anim_length = 10;	
				break;
			case DEAD:
				f->anim_seed = 25;
				f->anim_length = 1;
		}
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
	switch(atk)
		case ATK_N_P:
			f->anim_seed = 20; /*dont hardcode this*/
			f->anim_length = 4;
}

void TakeHit(Fighter* f, int dmg, int kback, int stun){
	f->health -= dmg;
	f->hitstun = stun;
//	f->y-=2;
}

void Die(Fighter* f){
	f->health=0;
	ChangeState(f,DEAD);
}
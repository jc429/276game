#include "fighter.h"


void MakeFighter(Fighter* f){
	
}
void LoadFighter(Fighter* f, Character_T c){
	char* temp;			//temporary thing we read from file
	char* filepath;

	/////Switch depending on character selected/////
	if(c==DOOM)
		filepath = "res/chr/doom.txt";
	else
		filepath = "res/chr/doom.txt";
	///////////////////////////////////////////////


	char* spritepath;
	char* hitpath;
	char* hurtpath;


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
#include "fighter.h"


void MakeFighter(Fighter* f){
	
}
void LoadFighter(Fighter* f, Character_T c){
	int t_width,t_height,t_per_row;
	t_width = 180;
	t_height = 150;
	t_per_row = 10;

	f->x_off = 90;
	f->y_off = 130;

	f->x = 30;
	f->y = 50;


	f->f_sprite = LoadSprite("images/idoom.png",t_width, t_height, t_per_row);
	if(f->f_sprite == NULL)fprintf(stdout,"No one can load Doom\n");
	f->f_hitbox = LoadSprite("images/idoom.png",t_width, t_height, t_per_row);
	if(f->f_hitbox == NULL)fprintf(stdout,"No one can load Doom\n");
	f->f_hurtbox = LoadSprite("images/idoom_hurt.png",t_width, t_height, t_per_row);
	if(f->f_hurtbox == NULL)fprintf(stdout,"No one can load Doom\n");


}
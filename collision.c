#include <string.h>
#include <stdlib.h>
#include "fighter.h"
#include "graphics.h"
#include "stage.h"
extern SDL_Surface *screen;

void CollisionCheck(Fighter* f){
	int debug_collision = 0;
	int hit,damage,angle,stun;
	int getxa,getya,getxd,getyd;
	Uint32 bestpx;
	Uint32 pxla;
	Uint32 pxld;
	int i,j;
	Uint8 blue,green,red,alpha; /* the RGBA values of the collision pixel*/

	if(f->state == ATTACKING){ /*the only collision type allowed so far*/
		SDL_Rect box_atk, box_def, box_col;
		box_atk.x = (f->x - f->x_off);
		box_atk.y = (f->y - f->y_off);
		box_atk.w = (f->t_width);
		box_atk.h = (f->t_height);
		box_def.x = (f->opponent->x - f->opponent->x_off);
		box_def.y = (f->opponent->y - f->opponent->y_off);
		box_def.w = (f->opponent->t_width);
		box_def.h = (f->opponent->t_height);
		
		if(((box_atk.x+box_atk.w) > box_def.x) && (box_atk.x < (box_def.x+box_def.w)) &&
		(box_atk.y < (box_def.y+box_def.h)) && ((box_atk.y+box_atk.h) > box_def.y)){
			
			box_col.x = Max(box_atk.x,box_def.x);
			box_col.y = Max(box_atk.y,box_def.y);
			box_col.w = (Min((box_atk.x+box_atk.w),(box_def.x+box_def.w))-box_col.x);
			box_col.h = (Min((box_atk.y+box_atk.h),(box_def.y+box_def.h))-box_col.y);

			
			bestpx = 0;
			pxla = 0;
			pxld = 0;
			
			
			
			for(i = 0; i < box_col.w; i++)
			{
				for(j = 0; j <  box_col.h; j++)
				{
					if(box_atk.x<box_def.x){
						getxa = (i + f->t_width - box_col.w)+(f->t_width*(f->frame%f->t_per_row));
						getxd = i+(f->opponent->t_width*(f->opponent->frame%f->opponent->t_per_row));
					}else{
						getxa = i+(f->t_width*(f->frame%f->t_per_row));
						getxd = (i + f->opponent->t_width - box_col.w)+(f->opponent->t_width*(f->opponent->frame%f->opponent->t_per_row));					
					}
					if(box_atk.y<box_def.y){
						getya = (j + f->t_height - box_col.h)+(f->t_height*(f->frame/f->t_per_row));
						getyd = j+(f->opponent->t_height*(f->opponent->frame/f->opponent->t_per_row));
					}
					else{
						getya = j+(f->t_height*(f->frame/f->t_per_row));
						getyd = (j + f->opponent->t_height - box_col.h)+(f->opponent->t_height*(f->opponent->frame/f->opponent->t_per_row));	
					}
					pxla = getpixel(f->f_hitbox->image,getxa,getya);/*oh god this works now just find the right position god bless Bo */
					pxld = getpixel(f->opponent->f_hurtbox->image,getxd,getyd);
					if((pxla & 0x00ffffff)==0x00ffffff) continue; /*if attacking pixel is blank don't waste time */
					if((pxld & 0x00ffffff)==0x00ffffff){
						continue; 
						/*eventually replace this with a check for the opponent's attack hitbox*/
					}

					if((pxla & 0x00ff0000) >> 16 > (bestpx & 0x00ff0000) >> 16){ /*for now just pick based off highest damage*/
						bestpx = pxla;
						hit = 1;
					}
					/* it would probably be ideal to go with lowest damage/stun, to encourage better positioning for attacks*/
					
				}
			}

			/*??????????????????????????????????????????????????????*/
			blue = (bestpx & 0x000000ff);		
			green = (bestpx & 0x0000ff00)>> 8;
			red = (bestpx & 0x00ff0000) >> 16;
			alpha = (bestpx & 0xff000000) >> 24;

			/* Do some math to make reasonable values out of the RGB ones*/
			damage = red/10;
			stun = blue/10;
			/*temp math - should be more fine-tuned eventually*/
			/*knockback is weird
			the G value will give the angle to knock the opponent back (from -90 to 165)
			and the force will be calculated somehow using hitstun and weight
			I haven't figured it out yet.
			*/
			angle = (green)-90;


			if(debug_collision&&hit){
				fprintf(stdout,"Red: %u\n", red);
				fprintf(stdout,"Grn: %u\n", green);
				fprintf(stdout,"Blu: %u\n", blue);
				fprintf(stdout,"Alpha: %u\n", alpha);
				fprintf(stdout,"pixel %u\n", bestpx);
				for(i = 0; i < box_col.w;i++){
					for(j=0; j<box_col.h;j++){ 
						DrawPoint(box_col.x+i,box_col.y+j);
					}
				}
			}
				/*??????????????????????????????????????????????????????*/
			if(damage>0&&stun>0)
				TakeHit(f->opponent,damage,angle,stun);

		}
	}
		
	
}

bool CollidePlatform(Fighter* f, Platform* p){
	if((f->y+f->vy > p->p_ypos)&&(f->x > p->p_left)
		&&(f->x < p->p_right)/*if we would be past the floor this frame*/
		&&((f->y)-p->p_ypos < 5)) /*if we're coming from above or really close from below*/
	{
		f->vy = 0;
		f->grounded = 1;
		f->hasjump = f->maxjumps;
		f->y = p->p_ypos;
		return true;
	}
	
	if(p->next)
		return CollidePlatform(f,p->next);
	else
		return false;
}


Uint16 Max(Uint16 a, Uint16 b){
	if (a >= b) return a;
	else return b;
}

Uint16 Min(Uint16 a, Uint16 b){
	if (a <= b) return a;
	else return b;
}


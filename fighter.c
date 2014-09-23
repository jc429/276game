#include "fighter.h"


void MakeFighter(Fighter* f){
	f->x = 30;
	f->y = 50;
	f->fsprite =  LoadSprite("images/idoom.png",180, 150, 10);
}

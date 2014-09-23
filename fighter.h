#ifndef __Fighter__
#define __Fighter__

#include "graphics.h"

typedef struct {
// Animation
	Sprite* f_sprite; //the actual graphics of the fighter (the part that will be visible during normal gameplay)
	Sprite* f_hitbox;	//the hitboxes of the fighter (used for dealing damage an determining damage/knockback/hitstun)
	Sprite* f_hurtbox;	//the hurtboxes of the fighter (the part that receives damage)
	int frame;			//what frame of animation we are on (in regards to the entire sprite sheet)
	int anim_seed;		//first frame of current animation
	int anim_length;	//length of current animation
	int facing; // -1 if facing left, 1 if facing right. 0 if anything else
	int state; // uses a state enum to check attacking, in block, in stun, idle, etc 
// Position/Movement
	int x,y; //current x and y position
	int prevx,prevy; //last non-special x and y (for throws and stuff)
	bool grounded;
	int jumptimer;	//used to prevent wasting all jumps immediately
	int hasjump;	//number of jumps remaining before you have to land
	int maxjumps;	//total number of jumps available
	int jumpspeed;
	int jumpheight;
	int weight;	//affects knockback, fall speed
	int fallspeed;
	int airdrift; //affects how much horizontal movement you can get in the air
	int walkspeed;
	int runspeed;
	int dashspeed; 
	int dashlength; //how far forward the dash goes before ending
// Combat
	int health;		//current health
	int healthmax;	//max health	
	int shield; // current strength of guard, affects chip damage and shield break
	int shieldmax;	// max strength of guard, affects chip damage and shield break
	int meter; //amount of meter in fuel gauge, used for movement options and hypers
	int hitstun; // hitstun timer - getting hit sets it to some number, hitstun decrements every frame.
	int shieldstun; //stun for when you hit a shield
	

} Fighter;

typedef enum{
	DOOM,NOTDOOM
} Character_T;



void MakeFighter();
void DrawFighters();
void LoadFighter(Fighter*,Character_T);

#endif
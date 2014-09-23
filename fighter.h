#ifndef __Fighter__
#define __Fighter__

#include "graphics.h"

typedef struct {
	Sprite* fsprite; //the actual graphics of the fighter (the part that will be visible during normal gameplay)
	Sprite* hitbox;	//the hitboxes of the fighter (used for dealing damage an determining damage/knockback/hitstun)
	Sprite* hurtbox;	//the hurtboxes of the fighter (the part that receives damage)
	int frame;	//what frame of animation we are on
	int facing; // -1 if facing left, 1 if facing right. 0 if anything else
	int x,y; //current x and y position
	int prevx,prevy; //last frame's x and y
	int health;
	int weight;	//affects knockback, fall speed
	int walkspeed;
	int runspeed;
	int dashspeed;
	int fallspeed;
	int jumpspeed;
	int airdrift; //affects how much horizontal movement you can get in the air
	int shield; // strength of guard, affects chip damage and sheild break
	int meter; //amount of meter in fuel gauge, used for movement options and hypers
	int hitstun; // hitstun timer - getting hit sets it to some number, hitstun decrements every frame.
	int shieldstun; //stun for when you hit a shield
	int state; // uses a state enum to check attacking, in block, in stun, idle, etc 

} Fighter;
void MakeFighter();
void DrawFighters();

#endif
#ifndef __Fighter__
#define __Fighter__

#include "graphics.h"

typedef enum{ /*List of Characters in the game*/
	NOTDOOM = 0, /*buggy test case*/
	DOOM = 1,
	WADDLE = 2
} Character_T;

typedef struct Fighter_T{
	Fighter_T* opponent;
	Character_T chr;/*what character are we?*/
	int controls;	/*used to lock the controls (0 = locked)*/
/* Animation*/
	int t_width,t_height,t_per_row; /*tile width, height, and num per row*/
	/*these 3 just point to either the left or right version of each sprite to make code simpler*/
	Sprite* f_sprite;	/*the actual graphics of the fighter (the part that will be visible during normal gameplay)*/
	Sprite* f_hitbox;	/*the hitboxes of the fighter (used for dealing damage an determining damage/knockback/hitstun)*/
	Sprite* f_hurtbox;	/*the hurtboxes of the fighter (the part that receives damage)*/
	Sprite* f_spritel;	/*facing left*/
	Sprite* f_spriter;	/*facing right*/
	Sprite* f_hitboxl;
	Sprite* f_hitboxr;
	Sprite* f_hurtboxl;
	Sprite* f_hurtboxr;	
	int frame;			/*what frame of animation we are on (in regards to the entire sprite sheet)*/
	int anim_seed;		/*first frame of current animation*/
	int anim_length;	/*length of current animation*/
	int facing;			/* -1 if facing left, 1 if facing right. 0 if anything else*/
	int state;			/*uses a state enum to check attacking, in block, in stun, idle, etc */
	int atktype;		/* which attack are we using? */
/* Position*/
	int x,y;			/*current x and y position*/
	int x_prev,y_prev;	/*last non-special x and y (for throws and stuff)*/
	int x_off,y_off;	/*offset from character's 'point' (usually middle of feet) to the top left corner of the frame)*/
	int bbox_w,bbox_h,bbox_x_off,bbox_y_off;	/*the rectangle of the player's bounding box (used for stage collision)*/
	int vx,vy;			/*x/y velocity/acceleration*/
	int accx,accy;
/* Movement*/
	int grounded;
	int jumptimer;	/*used to prevent wasting all jumps immediately*/
	int hasjump;	/*number of jumps remaining before you have to land*/
	int maxjumps;	/*total number of jumps available*/
	int jumpspeed;
	int jumpheight;
	int weight;	/*affects knockback, fall speed*/
	int fallspeed;
	int airdrift; /*affects how much horizontal movement you can get in the air*/
	int walkspeed;
	int runspeed;
	int crawlspeed;
	int dashspeed; 
	int dashlength; /*how far forward the dash goes before ending*/
	int airdashspeed;
	int airdashlength;
/* Combat*/
	int health;		/*current health*/
	int healthmax;	/*max health	*/
	int shield; /* current strength of guard, affects chip damage and shield break*/
	int shieldmax;	/* max strength of guard, affects chip damage and shield break*/
	int meter; /*amount of meter in fuel gauge, used for movement options and hypers*/
	int hitstun; /* hitstun timer - getting hit sets it to some number, hitstun decrements every frame.*/
	int shieldstun; /*stun for when you hit a shield*/
	int attackstun;	/*how long after initiating an attack that you can move*/

} Fighter;



typedef enum{
	INTRO,
	/*MOVEMENT*/
	IDLE = 0,			/*idle pose*/
	IDLE2,				/*mini-action if you stay idle too long*/
	WALKING,
	RUNNING,
	CROUCHING,
	CRAWLING_F,			/*maybe some characters can crawl*/
	CRAWLING_B,

	JUMPSQUAT,		
	JUMP_G_N,			/*jumping: ground or air, forward, neutral, or back*/
	JUMP_G_F,			/*may not need both grounded and aerial*/
	JUMP_G_B,
	JUMP_A_N,
	JUMP_A_F,
	JUMP_A_B,
	FALLING,
	LANDING,

	DASHING_F,
	DASHING_B,

	AIRDASH_F,			/*Air dashes don't all need to be filled (unless doing 8-way)*/
	AIRDASH_B,
	AIRDASH_U,
	AIRDASH_D,
	AIRDASH_U_F,
	AIRDASH_U_B,
	AIRDASH_D_F,
	AIRDASH_D_B,

	/*ATTACKS*/
	ATTACKING = 1,
	ATK_CLANK,				/*when two hitboxes collide but no hurtboxes do*/
	ATK_CLANK_AIR,

	/*GRABS*/
	/* 
	Upon a successful grab, the game becomes a rock-paper-scissors type game between grabber and victim.
	Grabber can throw forward, throw backward, or pummel and, if victim inputs the same action at the same time 
	(~3 frame leniency), the grab is broken and the victim escapes at an advantage.
	*/
	GRAB_ATTEMPT,
	GRAB_WHIFF,
	GRAB_HOOK,			/*when you have grabbed the foe, pulling them in for the grab hold*/
	GRAB_HOLD,			/*single frame/small loop for holding*/
	GRABBED_HELD,		/*for being held in a grab*/
	GRAB_PUMMEL,		/*pummeling the victim*/
	GRABBED_OOF,		/*getting pummeled*/
	GRAB_THROW_F,
	GRAB_THROW_B,		
	GRAB_BREAK,			/*two grabs at the same time = a grab break*/
	GRAB_RELEASE,		/*grabber releasing victim from grab*/
	GRABBED_FREE,		/*victim being freed from grab*/

	/*OTHER*/
	HIT = 2,
	DEAD = 3,
	BLOCKING,
	BLOCKING_CR,
	PUSHBLOCK,			/*will this even have pushblock?*/
	DODGING,			/*spot dodge?*/
	SMACKDOWN,			/*when you get knocked into the ground*/
	GETUP_N,			/*getting up after a combo, "teching" (neutral, forward, back)*/
	GETUP_F,
	GETUP_B,
	POPOUT_N,			/*popping out of a combo in midair*/
	POPOUT_F,
	POPOUT_B,

	TAUNT,
	TAUNT2
} State_T;

typedef enum{
	ATK_N_P,			/*neutral,forward,back,down,up + punch button, grounded*/
	ATK_F_P,			
	ATK_B_P,
	ATK_D_P,
	ATK_U_P,
	ATK_N_K,			/*neutral,forward,back,down,up + kick button, grounded*/
	ATK_F_K,
	ATK_B_K,
	ATK_D_K,
	ATK_U_K,
	ATK_AIR_N_P,			/*neutral,forward,back,down,up + punch button, in air*/
	ATK_AIR_F_P,			
	ATK_AIR_B_P,
	ATK_AIR_D_P,
	ATK_AIR_U_P,
	ATK_AIR_N_K,			/*neutral,forward,back,down,up + kick button, in air*/
	ATK_AIR_F_K,
	ATK_AIR_B_K,
	ATK_AIR_D_K,
	ATK_AIR_U_K
}Attack_T;

void Quit();

void InitFighters(Character_T p1, Character_T p2);
void ClearFighter(Fighter* f);
/*************************************************************/
void DrawFighters(SDL_Surface* screen);
void DrawHitboxes(SDL_Surface* screen);
void DrawHurtboxes(SDL_Surface* screen);
void DrawChar(Fighter* f,SDL_Surface* screen);
void UpdateFrame(Fighter* f);
void LoadFighter(Fighter*,Character_T);
/*************************************************************/
void FighterControl();
void FighterInputs(Fighter* f,Uint8 inputs);
void FighterThink(Fighter*);
void FighterUpdate(Fighter*);
/*************************************************************/
void CheckFacing(Fighter* f);
void DrawPlayerPoint(Fighter* f);
void DrawMeters(Fighter* f1,Fighter* f2);

void ChangeState(Fighter* f, State_T state);

void Jump(Fighter* f);
void Attack(Fighter* f,Attack_T atk);

void Update_All();

void CollisionCheck(Fighter* f);


void TakeHit(Fighter* f, int dmg, int kback, int stun);
void Die(Fighter* f);
void NextRound();
void GamePause();





Uint16 Min(Uint16 a, Uint16 b);
Uint16 Max(Uint16 a, Uint16 b);
#endif
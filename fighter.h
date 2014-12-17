#ifndef __Fighter__
#define __Fighter__

#include "graphics.h"
#include "stage.h"

/** List of Characters in the game*/
typedef enum{ 
	C1 = 0, /*buggy test case*/
	DEBUG = 0,
	C2 = 1,
	DOOM = 1,
	C3 = 2,
	WADDLE = 2,
	C4 = 3,
	MEGA = 3,
	C5 = 4,
	WIZ = 4,
	C6 = 5,
	C7 = 6,
	C8 = 7
} Character_T;



/** Fighter struct*/
typedef struct Fighter{
	struct Fighter *opponent;	/**< pointer to the other guy*/
	Character_T chr;			/**< what character are we?*/
	char* name;					/**< character name. I want to get rid of this, or at least not store it in the fighter struct*/
	Uint8 *inputs;				/**< pointer to the inputs of the player using this fighter*/
	int controls;				/**< used to lock the controls (0 = locked)*/
	Uint8 victories;			/**< for tracking rounds won*/
/* Animation*/
	int t_width,	/**< tile width*/
		t_height,	/**< tile height*/
		t_per_row;	/**< num tiles per row on spritesheet*/
	/*these 3 just point to either the left or right version of each sprite to make code simpler*/
	Sprite* f_sprite;	/**< the actual graphics of the fighter (the part that will be visible during normal gameplay)*/
	Sprite* f_hitbox;	/**< the hitboxes of the fighter (used for dealing damage an determining damage/knockback/hitstun)*/
	Sprite* f_hurtbox;	/**< the hurtboxes of the fighter (the part that receives damage)*/
	Sprite* f_spritel;	/**< sprite facing left*/
	Sprite* f_spriter;	/**< sprite facing right*/
	Sprite* f_hitboxl;	/**< hitbox facing left*/
	Sprite* f_hitboxr;	/**< hitbox facing right*/
	Sprite* f_hurtboxl; /**< hurtbox facing left*/
	Sprite* f_hurtboxr;	/**< hurtbox facing right*/
	int frame;			/**< what frame of animation we are on (in regards to the entire sprite sheet)*/
	int anim_seed;		/**< first frame of current animation*/
	int anim_length;	/**< length of current animation*/
	int facing;			/**< -1 if facing left, 1 if facing right. 0 if anything else*/
	int state;			/**< uses a state enum to check attacking, in block, in stun, idle, etc */
	int atktype;		/**< which attack are we using? */
/* Position*/
	int x,y;			/**< current x and y position*/
	int x_prev,y_prev;	/**< last non-special x and y (for throws and stuff)*/
	int x_off,y_off;	/**< offset from character's 'point' (usually middle of feet) to the top left corner of the frame)*/
	int bbox_w,bbox_h,bbox_x_off,bbox_y_off;	/**< the rectangle of the player's bounding box (used for stage collision)*/
	int vx,vy;			/**< x/y velocity/acceleration*/
	int accx,accy;
/* Movement*/
	int grounded;	/**< "bool" to check if grounded*/
	int jumptimer;	/**< used to prevent wasting all jumps immediately*/
	int hasjump;	/**< number of jumps remaining before you have to land*/
	int maxjumps;	/**< total number of jumps available*/
	int jumpspeed;	/**< how fast the fighter jumps*/
	int jumpheight;	/**< how high the fighter can jump*/
	int weight;		/**< affects knockback, fall speed*/
	int fallspeed;	/**< this should be obvious*/
	int airdrift;	/*affects how much horizontal movement you can get in the air*/
	int walkspeed;	/**< walk speed*/
	int runspeed;	/**< run speed*/
	int crawlspeed;	/**< speed while holding down+forward (if the character can crouch)*/
	int dashspeed;	/**< speed of a dash*/
	int dashlength; /**< how far forward the dash goes before ending*/
	int airdashspeed;	/**< speed of an airdash*/
	int airdashlength;	/**< length of an air dash*/
/* Combat*/
	int health;		/**< current health*/
	int healthmax;	/**< max health	*/
	int shield;		/**< current strength of guard, affects chip damage and shield break*/
	int shieldmax;	/**< max strength of guard, affects chip damage and shield break*/
	int meter;		/**< amount of meter in fuel gauge, used for movement options and hypers*/
	int hitstun;	/**< hitstun timer - getting hit sets it to some number, hitstun decrements every frame.*/
	int shieldstun; /**< stun for when you hit a shield*/
	int attackstun;	/**< how long after initiating an attack that you must wait before you can move/cancel the animation*/
	int combo;		/**< what hit of a combo we're currently on, resets when hitstun = 0*/ 
} Fighter;


/** Fighter States */
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
	HIT = 3,
	DEAD = 4,
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
	ATK_N_P = 0,			/*neutral,forward,back,down,up + punch button, grounded*/
	ATK_F_P,			
	ATK_B_P,
	ATK_D_P,
	ATK_U_P,
	ATK_N_K = 1,			/*neutral,forward,back,down,up + kick button, grounded*/
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
char* GetCharPath(int c);
int LoadCFG(Fighter* f,char* path);
int SaveCFG(Fighter* f,char* path);
/*************************************************************/
void InputControl();
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

void CollisionCheck(Fighter* f);
bool CollidePlatform(Fighter* f, Platform* p);

void TakeHit(Fighter* f, int dmg, int kback, int stun);
void Die(Fighter* f);





Uint16 Min(Uint16 a, Uint16 b);
Uint16 Max(Uint16 a, Uint16 b);
#endif
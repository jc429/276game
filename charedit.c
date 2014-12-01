#include "game.h"

#define NUMCHARS 8 /*number of characters in the game*/
#define NUMANIMS 5 /*animations per character*/

Fighter *efighter;
extern GameState_T GameState;

int framedata[NUMCHARS][NUMANIMS] = {  
	/*idle, idle2, walk, run, crouch,jumpsq,jump,fall,land,dashf,dashb, */
	/*For Now: {idle,attack1,attack2,hit,dead}*/
	/*each int denotes the starting point for the animation, in a set order*/
	/*MAKE SURE EVERY INT IS LARGER THAN THE ONE BEFORE IT*/
	/*there needs to be a final int that isn't a seed, for the final animation's length*/
	/*DEBUG*/{0, 1, 2, 3, 4} ,   /*  frames for character 1 */
	/*DOOM*/{0, 20, 24, 28, 29} ,   /*  ex: column 1 is idle, then idle2, then attacking, then hit, etc */
	/*WADDLE*/{0, 4, 11, 15, 16},
	{0, 1, 2, 3, 4},
	{0, 1, 2, 3, 4},
	{0, 1, 2, 3, 4},
	{0, 1, 2, 3, 4},
	{0, 1, 2, 3, 4}
	
};

void GoToCharCr(){
	GameState = C_CREATOR;
	efighter = (Fighter*)malloc(sizeof(Fighter));
}

void DrawCharCr(){
	DrawBG("images/menubg2.png");

}

void UpdateCharCr(){
	
}
/*
 *    Donald Kehoe
 *    Sometime in Fenruary
 *    Last Modified: 3/3/05
 *
 *    Description: definitions for Entity handling functions (methods).
 *      Definitions for specific instances of Entities for this simple game.
*/
#include <stdlib.h>
#include <math.h>
#include "entity.h"
/*
  Entity function definitions
*/

enum FACTION {E_NONE,E_Player,E_Bugs};
extern SDL_Surface *screen;
extern SDL_Event Event; //This is just so the think functions in this file can read what has been input.
extern SDL_Rect Camera;
extern int NumLevels;
extern int CurrentLevel;
Entity EntityList[MAXENTITIES];
Entity *Player;
int NumLives = 3;
int NumEnts;
int MOUSEMOVE = 1;
int lastx,lasty; /*last mouse coordinates*/

enum BlasterTypes {BL_Red,BL_Blue,BL_Purple,BL_Blue2,BL_Blue3,BL_Teal};

void DrawEntities()
{
  int i;
  for(i = 0; i < MAXENTITIES;i++)
  {
    if(EntityList[i].used)
    {
      if(EntityList[i].shown)
        DrawEntity(&EntityList[i]);
    }
  }
}

void UpdateEntities()
{
  int i;
  for(i = 0;i < MAXENTITIES;i++)
  {
    if(EntityList[i].used)
    {
      if(EntityList[i].think != NULL)
      {
        EntityList[i].think(&EntityList[i]);
      }
    }
  }
}

void DrawEntity(Entity *ent)
{
  DrawSprite(ent->sprite,screen,ent->sx,ent->sy,ent->frame);
}

void InitEntityList()
{
  int i,j;
  NumEnts = 0;
  for(i = 0;i < MAXENTITIES; i++)
  {
    EntityList[i].sprite = NULL;
    EntityList[i].owner = NULL;
    EntityList[i].think = NULL;
    for(j = 0;j < SOUNDSPERENT;j++)
    {
      EntityList[i].sound[j] = NULL;
    }
    EntityList[i].shown = 0;
    EntityList[i].used = 0;
  }
}

/*
  returns NULL if all filled up, or a pointer to a newly designated Entity.
  Its up to the other function to define the data.
*/
Entity *NewEntity()
{
  int i;
  if(NumEnts + 1 >= MAXENTITIES)
  {
    return NULL;
  }
  NumEnts++;
  for(i = 0;i <= NumEnts;i++)
  {
    if(!EntityList[i].used)break;
  }
  EntityList[i].used = 1;
  return &EntityList[i];
}

/*done with an entity, now give back its water..I mean resources*/
void FreeEntity(Entity *ent)
{
  int j;
  ent->used = 0;
  NumEnts--;
  if(ent->sprite != NULL)FreeSprite(ent->sprite);
  for(j = 0;j < SOUNDSPERENT;j++)
  {
    if(ent->sound[j] != NULL)FreeSound(ent->sound[j]);
    ent->sound[j] = NULL;
  }
  ent->sprite = NULL;
  ent->owner = NULL;
}

/*kill them all*/
void ClearEntities()
{
  int i = 0;
  for(i = 0;i < MAXENTITIES;i++)
  {
    FreeEntity(&EntityList[i]);
  }
}

/*
 * This handly little function will figure pace out a looping animation for an entity.
 * It could probably have been a macro...oh well.
 */
int GetNextCount(Entity *self)
{
  return (self->count+1)%(self->framestates[self->state + 1] - self->framestates[self->state]);
}

void ScaleVectors(float *vx, float *vy)
{
  float hyp;
  hyp = sqrt((*vx * *vx) + (*vy * *vy));
  hyp = 1 / hyp;
  *vx = (*vx * hyp);
  *vy = (*vy * hyp);
}

int Collide(SDL_Rect box1,SDL_Rect box2)
{
  /*check to see if box 1 and box 2 clip, then check to see if box1 is in box or vice versa*/
  if((box1.x + box1.w >= box2.x) && (box1.x <= box2.x+box2.w) && (box1.y + box1.h >= box2.y) && (box1.y <= box2.y+box2.h))
    return 1;
  return 0;
}

Entity *HitNextEnt(Entity *self,Entity *target)
{
  int i;
  SDL_Rect b1,b2;
  b1.x = self->sx + self->bbox.x;
  b1.y = self->sy + self->bbox.y;
  b1.w = self->bbox.w;
  b1.h = self->bbox.h;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(target == &EntityList[i])break;
  }
  for(i = i;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if((EntityList[i].health > 0)&&(EntityList[i].enemy != E_NONE)&&(EntityList[i].enemy != self->enemy))
      {
        b2.x = EntityList[i].sx + EntityList[i].bbox.x;
        b2.y = EntityList[i].sy + EntityList[i].bbox.y;
        b2.w = EntityList[i].bbox.w;
        b2.h = EntityList[i].bbox.h;
        if(Collide(b1,b2))
         return &EntityList[i];
      }
    }
  }
  return NULL;
}

Entity *GetClosestTarget(Entity *self)
{
  int i;
  int range = 2000;
  int Trange = 2000;
  Entity *target = NULL;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if((EntityList[i].health > 0)&&(EntityList[i].enemy != E_NONE)&&(EntityList[i].enemy != self->enemy))
      {
        Trange = /*sqrt*/((EntityList[i].sx - self->sx)*(EntityList[i].sx - self->sx) + (EntityList[i].sy - self->sy)*(EntityList[i].sy - self->sy));
        if(Trange < range)
        {
          target = &EntityList[i];
          range = Trange;
        }
      }
    }
  }
  return target;
}


Entity *HitEnt(Entity *self)
{
  int i;
  SDL_Rect b1,b2;
  b1.x = self->sx + self->bbox.x;
  b1.y = self->sy + self->bbox.y;
  b1.w = self->bbox.w;
  b1.h = self->bbox.h;
  for(i = 0;i < MAXENTITIES; i++)
  {
    if(EntityList[i].used)
    {
      if((EntityList[i].health > 0)&&(EntityList[i].enemy != E_NONE)&&(EntityList[i].enemy != self->enemy))
      {
        b2.x = EntityList[i].sx + EntityList[i].bbox.x;
        b2.y = EntityList[i].sy + EntityList[i].bbox.y;
        b2.w = EntityList[i].bbox.w;
        b2.h = EntityList[i].bbox.h;
        if(Collide(b1,b2))
         return &EntityList[i];
      }
    }
  }
  return NULL;
}


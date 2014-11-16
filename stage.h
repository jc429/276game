#ifndef _STAGE_
#define _STAGE_

typedef enum {
	ST_FIELD=0,
	ST_PLATFORM=1,
	ST_DEBUG=2
} StageList;

typedef struct Platform{
	int p_left;
	int p_right;
	int p_ypos;
	Platform* next;
} Platform;

typedef struct StageLayer{
	int movescale;		/*how much this layer moves in relation to the platforms - first layer(bg) should be 0, platforms layer should be 1*/
	int x,y,w,h;
	int offx,offy;
	char* layerpath;
	StageLayer* next;
} StageLayer;

typedef struct Stage{
	char* st_name;
	char* bg;
	StageLayer* layer_list;
	Platform* platform_list;
	int P1spawn;
	int P2spawn;
} Stage;

void LoadStage(StageList stage);
void UpdateStage();
void DrawStage(StageList stage);
void DrawLayer(StageLayer* layer);


#endif
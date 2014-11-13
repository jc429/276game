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
	int depth;
	int x,y;
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



#endif
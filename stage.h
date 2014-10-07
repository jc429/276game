#ifndef _STAGE_
#define _STAGE_

typedef enum {
	ST_FIELD=0,
	ST_PLATFORM=1,
	ST_DEBOOG=2
} StageList;


void LoadStage(StageList stage);
void UpdateStage();
void DrawStage(StageList stage);
void DrawStageBG(char* bgloc);


#endif
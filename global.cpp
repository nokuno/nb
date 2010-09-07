//global.cpp
//グローバル変数の実体生成用
#include "stdapp.h"
#include "nb.h"

CMainGame* g_pcMainGame;
int MAP_SIZE_X;
int MAP_SIZE_Y;
int MAP_X;
int MAP_Y;
int MAP_SQUARE;
int MAP_SIZE;
byte pdevstate[PL_MAX][DEV_MAX];
int devevent[PL_MAX][DEV_MAX];
int g_fChange=0;//変化フラグ（ＡＩの評価ポイントを決定）
int g_time=0;//時間
vector<int> g_value;	//AI用場所あたりの評価点
char szStageFile[32];
char szIniFile[] = "system.dat";
CPlayer		g_player[5];
CMysMidiEx*	g_pMidi = NULL;
CObj**		g_ppObj;
CMap**		g_ppMap;

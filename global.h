
#ifndef _INC_GLOBAL_H_
#define _INC_GLOBAL_H_

#define FLAG_TRUE -1
#define FLAG_FALSE -2
#define FLAG_UNINI -3

extern GApp gapp;
extern CMysLib2* g_pcMysLib;
extern CMysInput2* g_pcMysInput;
extern CMainGame* g_pcMainGame;

extern int MAP_SIZE_X;
extern int MAP_SIZE_Y;
extern int MAP_X;
extern int MAP_Y;
extern int MAP_SQUARE;
extern int MAP_SIZE;
extern byte pdevstate[PL_MAX][DEV_MAX];//��t���[���O�̓���
extern int devevent[PL_MAX][DEV_MAX];//�����ꂽ�u��1,�����ꂽ�u��-1
extern int g_fChange;
extern int g_time;
extern vector<int> g_value;	//AI�p�ꏊ������̕]���_
extern char szStageFile[];
extern char szIniFile[];
extern CPlayer			g_player[5];
extern CMysMidiEx*		g_pMidi;
extern CObj**			g_ppObj;
extern CMap**			g_ppMap;

#endif

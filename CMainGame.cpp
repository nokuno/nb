#include "stdapp.h"
#include "nb.h"
/********************************************************************************

	初期化（プロファイル読み込み等）

********************************************************************************/
bool CMainGame::Initialize()
{
	try
	{
	g_pcMysLib->WaitTime(1000,true,true);
	char buffer[32];
	int i,j;
	srand((unsigned int)time(NULL));
	if(!LoadConfig())//キーコンフィグ読み込み
	{
		MessageBox(NULL,"error","config",MB_OK);
		SetDefaultConfig();
	}
	CMysImageEx::ClearBuffer();
	CMysImageEx::ClearList();
	MAP_SIZE_X = prfGetInt(szIniFile,"config","map_size_x");
	MAP_SIZE_Y = prfGetInt(szIniFile,"config","map_size_y");
	MAP_X = prfGetInt(szIniFile,"config","map_x");
	MAP_Y = prfGetInt(szIniFile,"config","map_y");
	//ブロックのおかれうる全てのマスの数
	MAP_SQUARE = (MAP_SIZE_X-2) * (MAP_SIZE_Y-2) - (MAP_SIZE_X-3)/2 * (MAP_SIZE_Y-3)/2;
	//マップサイズ
	MAP_SIZE = MAP_SIZE_X * MAP_SIZE_Y;
	g_time = 0;
	g_value.assign(MAP_SIZE,0);
	CPlayer::InitStatic();
	for(i=0;i<5;i++)
		g_player[i].Initialize(i);
	InitObj();
	InitMap();
	for(i=0;i<5;i++)
		for(j=0;j<DEV_MAX;j++)
			devstate[i][j]=0;
	GetDevEvent();
	m_cimgBlock1.LoadImage(prfGetString(szStageFile,"img","block1",buffer));
	m_cimgBlock1.CreateMask();
	m_cFpsImage.LoadNumberImage("img\\fps.png",true);
	
	g_pMidi = new CMysMidiEx;
	g_pMidi->LoadMidi(prfGetString(szStageFile,"bgm","bgm",buffer));
	g_pMidi->SetLoop(true,4);
	g_pMidi->Play();
	return true;
	}
	catch(...)
	{
		MessageBox("Initialize");
	}
}
/********************************************************************************

	終了

********************************************************************************/
void CMainGame::EndGame()
{
	CMysSoundEx::DeleteAll();
	for(int i=0;i<MAP_SIZE;i++)
		if(g_ppObj[i] != NULL)
			delete g_ppObj[i];
	delete[] g_ppObj;
	for(i=0;i<MAP_SIZE;i++)
		if(g_ppMap[i] != NULL)
			delete g_ppMap[i];
	delete[] g_ppMap;
#ifndef _DEBUG
	g_pMidi->Stop();
	g_pMidi->Delete();
	delete g_pMidi;
#endif
}
/********************************************************************************

	ステップフレーム

********************************************************************************/
bool CMainGame::StepFrame()
{
	try
	{
	for(int i=0;i<MAP_SIZE;i++)
		if(g_ppObj[i] != NULL)
			g_ppObj[i]->StepFrame();
		
	GetDevEvent();//キー状態更新
	int ef=0;
	for(i=0;i<5;i++)
		if(g_player[i].GetFlag())
		{
			ef++;
			g_player[i].StepFrame();
		}
	if(ef<=1)
		return false;
	if(g_fChange)
		g_fChange --;
	g_time ++;
	return true;
	}
	catch(...)
	{
		MessageBox("StepFrame");
	}
}
/********************************************************************************

	全描画

********************************************************************************/
void CMainGame::Render()
{
	int i;
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	if(sysini.bWaitVBI) g_pcMysLib->Render();		//VBI待ちの都合上先にやる

	for(i=0;i<MAP_SIZE;i++)
		if(g_ppMap[i] != NULL)
			g_ppMap[i]->Render();
	for(i=0;i<MAP_SIZE;i++)
		if(g_ppObj[i] != NULL)
			g_ppObj[i]->Render();
	for(i=0;i<5;i++)
		if(g_player[i].GetFlag())
			g_player[i].Render();
	CMysImageEx::BltAll();
	DrawFloor1();
	DrawFloor2();


	//FPS計測
#ifdef _DEBUG
	static DWORD dwOldTime=timeGetTime();
	DWORD dwTime=0;
	static DWORD dwFPS=0;
	static DWORD dwCount=0;
	static char ss[20] = "";
	dwCount++;
	dwTime = timeGetTime();
	if(dwTime-dwOldTime >= 1000)
	{
		dwFPS = 1000*dwCount/(dwTime-dwOldTime);
		dwCount = 0;
		sprintf(ss,"%3d",dwFPS);
		dwOldTime = timeGetTime();
	}
	m_cFpsImage.BltNumberFast(NULL,dwFPS,4,0,0);
#endif
	if(!sysini.bWaitVBI) g_pcMysLib->Render();

}
/********************************************************************************

	床描画

********************************************************************************/
void CMainGame::DrawFloor1(void)//下レイヤー
{
	for(int i=0;i<MAP_SIZE_X;i+=1)
	{
		for(int j=0;j<MAP_SIZE_Y;j+=1)
		{
			RECT rect = {0,32,32,64};
			if(!(
				(i%2==1||j%2==1)&&
				(i!=0&&j!=0&&i!=MAP_SIZE_X-1&&j!=MAP_SIZE_Y-1)
				))
			{
				m_cimgBlock1.BltRsv(-50,MAP_X+i*32,MAP_Y+j*32,&rect);
			}
		}
	}
}
void CMainGame::DrawFloor2(void)//上レイヤー
{
	for(int i=0;i<MAP_SIZE_X;i+=1)
	{
		for(int j=0;j<MAP_SIZE_Y;j+=1)
		{
			RECT rect = {0,0,32,32};
			if((
				(i%2==1||j%2==1)&&
				(i!=0&&j!=0&&i!=MAP_SIZE_X-1&&j!=MAP_SIZE_Y-1)
				)==FALSE
				)
			{
				m_cimgBlock1.mysBlt(NULL,MAP_X+i*32,MAP_Y+(j-1)*32,&rect);
			}
		}
	}
}

/********************************************************************************

	スタート関数（グローバル）

********************************************************************************/
bool StartMainGame()
{
	g_pcMainGame = new CMainGame;	//メインゲームクラス
	if(g_pcMainGame==NULL) return false;

	bool ret = g_pcMainGame->Main();

	g_pcMainGame->EndGame();
	DELETE_PTR(g_pcMainGame);		//メインゲームクラス解放

	return ret;
}
/********************************************************************************

	メイン関数

********************************************************************************/
// タイトルに戻る場合はtrueを返す
// アプリケーションを終了する場合はfalseを返す
bool CMainGame::Main()
{
	//垂直帰線同期を取るモードかどうかをチェックし、それに応じて待つ時間を設定する
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	int waittime;	//次フレームまで待つ時間
	if(sysini.bWaitVBI) waittime = 1000/40;	//若干多めにとる
	else waittime = 1000/30;

	Initialize();	//メインゲームクラス初期化

	bool ret = false;	//戻り値
	//メインゲームループ
	while(true)
	{
		if(sysini.bWaitVBI) Render();
		if(!StepFrame()) { ret=true; break; }
		if(!sysini.bWaitVBI) Render();
		if(!g_pcMysLib->WaitTime(waittime,true,true)) { ret=false; break; }
	}
	return ret;
}
//押された瞬間を検出
void GetDevEvent()
{
	int i,j;
	for(i=0;i<5;i++)
		for(j=0;j<DEV_MAX;j++)
			pdevstate[i][j] = devstate[i][j];
	GetDevState();//キー状態更新
	for(i=0;i<5;i++)
		for(j=0;j<DEV_MAX;j++)
			if(devstate[i][j]&&!pdevstate[i][j])
				devevent[i][j] = 1;
			else if(!devstate[i][j]&&pdevstate[i][j])
				devevent[i][j] = -1;
			else
				devevent[i][j] = 0;
}

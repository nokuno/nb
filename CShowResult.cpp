#include "stdapp.h"
#include "nb.h"

bool CShowResult::StepFrame()
{
	GetDevEvent();//キー状態更新
	if(devevent[PL_1][DEV_B1]==1)
		OnOk();
	return true;
}
bool CShowResult::Initialize()
{
	char buffer[128];
	m_snd.LoadSound(prfGetString(szIniFile,"snd","fanfare",buffer));
	m_snd.Play();
	m_timer=100;
	return true;
}
void CShowResult::Render()
{
	if(m_timer>0)
		m_timer--;
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	if(sysini.bWaitVBI) g_pcMysLib->Render();		//VBI待ちの都合上先にやる

	int ef=0;
	int pnum;
	for(int i=0;i<5;i++)
	{
		if(g_player[i].GetFlag())
		{
			ef++;
			pnum = i;
		}
	}
	RECT r = {96,64,432,128};
	if(ef == 0)//引き分け
		s_imgStrs.mysBlt(NULL,200,200,&r);//DRAW
	if(ef == 1)//勝ち
	{
		RECT r1 = {96, 0,96+256,64};
		s_imgStrs.mysBlt(NULL,250,200,&r1);//WIN!
		RECT r2 = {0,pnum*64,96,pnum*64+64};
		s_imgStrs.mysBlt(NULL,100,200,&r2);//ｘＰ
	}
	if(!sysini.bWaitVBI) g_pcMysLib->Render();
}
void CShowResult::OnOk()
{
	char buffer[32];
	if(m_timer==0)
	{
		pcActive = pcSelectStage;
#ifndef _DEBUG
		g_pMidi = new CMysMidiEx;
		g_pMidi->LoadMidi(prfGetString(szIniFile,"bgm","menu",buffer));
		g_pMidi->SetLoop(true,4);
		g_pMidi->Play();
#endif _DEBUG
	}
}
#include "stdapp.h"
#include "nb.h"

bool CSelectStage::StepFrame()
{
	GetDevEvent();//キー状態更新
	if(devevent[PL_1][DEV_L]==1)
		OnLeft();
	if(devevent[PL_1][DEV_R]==1)
		OnRight();
	if(devevent[PL_1][DEV_B1]==1)
	{
		OnOk();
		return false;
	}
	if(devevent[PL_1][DEV_B2]==1)
		OnCancel();
	return true;
}
bool CSelectStage::Initialize()
{
	m_cursor = 0;
	ConcludeStage();
	return true;
}
void CSelectStage::Render()
{
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	if(sysini.bWaitVBI) g_pcMysLib->Render();		//VBI待ちの都合上先にやる

	s_imgBG.mysBlt(NULL,0,0);
	RECT r1 = {96,128,384,176};
	s_imgStrs.mysBlt(NULL,160,50,&r1);//stage select

	RECT r2 = {96,336,144,368};
	s_imgStrs.mysBlt(NULL,20,400,&r2);//←
	RECT r3 = {144,336,192,668};
	s_imgStrs.mysBlt(NULL,640-20-48,400,&r3);//→
	m_prev.mysBlt(NULL,170,100);//preview
	if(!sysini.bWaitVBI) g_pcMysLib->Render();
}
void CSelectStage::OnLeft()
{
	if(m_cursor>0)
		m_cursor--;
	else
		m_cursor=STAGE_NUM-1;
	ConcludeStage();
}
void CSelectStage::OnRight()
{
	if(m_cursor<STAGE_NUM-1)
		m_cursor++;
	else
		m_cursor=0;
	ConcludeStage();
}
void CSelectStage::OnOk()
{
	ConcludeStage();
}
void CSelectStage::OnCancel()
{
	pcActive = pcSelectPlayer;
}
void CSelectStage::ConcludeStage()
{
	sprintf(szStageFile,"stg/%d.nbs",m_cursor+1);//ステージファイルパスを決定
	char buffer[32];
	m_prev.LoadImage(prfGetString(szStageFile,"img","prev",buffer));
}

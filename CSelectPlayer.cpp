#include "stdapp.h"
#include "nb.h"

bool CSelectPlayer::StepFrame()
{
	GetDevEvent();//ÉLÅ[èÛë‘çXêV
	if(devevent[PL_1][DEV_U]==1)
		OnUp();
	if(devevent[PL_1][DEV_D]==1)
		OnDown();
	if(devevent[PL_1][DEV_L]==1)
		OnLeft();
	if(devevent[PL_1][DEV_R]==1)
		OnRight();
	if(devevent[PL_1][DEV_B1]==1)
		OnOk();
	if(devevent[PL_1][DEV_B2]==1)
		OnCancel();
	return true;
}
bool CSelectPlayer::Initialize()
{
	GetDevEvent();
	char buffer[32];
	char strPlayer[32];//"playeri"
	for(int i=0;i<5;i++)
	{
		sprintf( strPlayer, "player%d", i );
		//ÇbÇoÇtÇ©ÅAë∂ç›Ç∑ÇÈÇ©
		prfGetString(szIniFile,"config",strPlayer,buffer);
		if(strcmp(buffer,"none")==0)			m_plstate[i] = 2;//none
		else if(strcmp(buffer,"computer")==0)	m_plstate[i] = 1;//computer
		else									m_plstate[i] = 0;//human
	}
	m_cursor = 0;
	return true;
}
void CSelectPlayer::Render()
{
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	if(sysini.bWaitVBI) g_pcMysLib->Render();		//VBIë“ÇøÇÃìsçáè„êÊÇ…Ç‚ÇÈ

	s_imgBG.mysBlt(NULL,0,0);
	RECT r1 = {0,0,96,320};
	s_imgStrs.mysBlt(NULL,140,50,&r1);//ÇòÇo

	RECT rect;
	rect.left = 0;
	rect.right = 80;
	for(int i=0;i<5;i++)
	{
		switch(m_plstate[i])
		{
		case 0://human
			rect.top = 320;
			rect.bottom = 368;
			break;
		case 1://com
			rect.top = 368;
			rect.bottom = 416;
			break;
		case 2://none
			rect.top = 416;
			rect.bottom = 464;
			break;
		}
		s_imgStrs.mysBlt(NULL,300,50+64*i,&rect);
	}
	RECT rc = {96,368,144,416};
	s_imgStrs.mysBlt(NULL,100,50+64*m_cursor,&rc);//ÉJÅ[É\Éã
	
	if(!sysini.bWaitVBI) g_pcMysLib->Render();
}
void CSelectPlayer::OnUp()
{
	if(m_cursor>0)
		m_cursor --;
}
void CSelectPlayer::OnDown()
{
	if(m_cursor<4)
		m_cursor ++;
}
void CSelectPlayer::OnLeft()
{
	if(m_plstate[m_cursor]>0)
		m_plstate[m_cursor]--;
}
void CSelectPlayer::OnRight()
{
	if(m_plstate[m_cursor]<2)
		m_plstate[m_cursor]++;
}
void CSelectPlayer::OnOk()
{
	CMysImageEx::ClearBuffer();
	int offNum = 0;//off number
	for(int i=0;i<5;i++)
	{
		char strPlayer[32];//"playeri"
		sprintf( strPlayer, "player%d", i );
		char buffer[32];

		switch(m_plstate[i])
		{
		case 0:
			strcpy(buffer,"human");
			prfSetString(szIniFile,"config",strPlayer,buffer);
			break;
		case 1:
			strcpy(buffer,"computer");
			prfSetString(szIniFile,"config",strPlayer,buffer);
			break;
		case 2:
			strcpy(buffer,"none");
			prfSetString(szIniFile,"config",strPlayer,buffer);
			offNum ++;
			break;
		}
	}
	if(offNum<4)
		pcActive = pcSelectStage;
}
void CSelectPlayer::OnCancel()
{
	pcActive = pcTitle;
#ifndef _DEBUG
	if(g_pMidi != NULL)
	{
		g_pMidi->Stop();
		g_pMidi->Delete();
		DELETE_PTR(g_pMidi);
	}
#endif
}

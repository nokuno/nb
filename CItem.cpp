#include "stdapp.h"
#include "nb.h"

const int CItem::MAX_ANMCOUNT=6;
const int CItem::MAX_ANMFRAME=4;
CMysImageEx CItem::s_img;
CMysSoundEx CItem::s_snd;

CItem::CItem(int kind,int x,int y)
{
	g_fChange = 1;//•Ï‰»ƒtƒ‰ƒO‚ð‚½‚Ä‚é
	m_kind=kind;
	m_x=x;m_y=y;
	g_ppObj[Index(x,y)] = this;
	m_anmCount = 0;
	m_anmFrame = 0;
	m_anmDirect = 1;
}
void CItem::LoadImage()
{
	char buffer[32];
	s_img.LoadImage(prfGetString(szIniFile,"img","item",buffer));
	s_img.CreateMask();
	s_snd.LoadSound(prfGetString(szIniFile,"snd","item",buffer));
}
void CItem::StepFrame()
{
	m_anmCount ++;
	if(m_anmCount == MAX_ANMCOUNT)
	{
		m_anmCount = 0;
		if(m_anmFrame == MAX_ANMFRAME-1)
			m_anmDirect = - m_anmDirect;
		else if(m_anmDirect==-1 && m_anmFrame == 0)
			m_anmDirect = - m_anmDirect;
		m_anmFrame += m_anmDirect;
	}
}
void CItem::Render()
{
	RECT rect;
	rect.left = m_anmFrame * 32;
	rect.right = (m_anmFrame+1) * 32;
	rect.top = m_kind * 32;
	rect.bottom = (m_kind+1) * 32;
	s_img.BltRsv(m_y*32,MAP_X+m_x*32,MAP_Y+m_y*32,&rect);
}

bool CItem::ColPlayer(CPlayer* pl)
{
	return true;
}
bool CItem::OnPlayer(CPlayer* pl)
{
	s_snd.Play();
	switch(m_kind)
	{
	case ITEM_SPEED:
		pl->IncSpeed();
		break;
	case ITEM_FIRE:
		pl->IncFire();
		break;
	case ITEM_BOMB:
		pl->IncBomb();
		break;
	}
	g_ppObj[Index(m_x,m_y)]=NULL;
	delete this;
	return true;
}
bool CItem::ColFire(int bombIndex)
{
	g_ppObj[Index(m_x,m_y)] = new CErase(FLAG_UNINI,m_x,m_y,bombIndex);
	delete this;
	return false;
}

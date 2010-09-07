#include "stdapp.h"
#include "nb.h"

const int CFire::MAX_ANM_COUNT=4;
CMysImageEx CFire::s_img;

void CFire::LoadImage()
{
	char buffer[32];
	s_img.LoadImage(prfGetString(szIniFile,"img","fire",buffer));
	s_img.CreateMask();
}
CFire::CFire(FIREKIND ak,int x,int y,int bombIndex,DIRECTION direct)
{
	m_anmKind = ak;
	m_renderKind = ak;
	m_x = x;
	m_y = y;
	m_bombIndex = bombIndex;
	m_direct = direct;
	m_anmCount = 0;
	m_anmFrame = 0;
	g_ppObj[Index(x,y)] = this;
}
void CFire::StepFrame()
{
	//Ç±Ç±Ç©ÇÁå©ÇΩñ⁄ÇÃêﬂä÷åW
	m_renderKind = m_anmKind;
	CObj *pObj;
	switch(m_anmKind)
	{
	case FIRE_UD:
		pObj = g_ppObj[Index(m_x-1,m_y)];
		if(pObj!=NULL&&pObj->IsFire()&&((CFire*)pObj)->GetFireKind()!=FIRE_R)
			m_renderKind = FIRE_CENTER;
		pObj = g_ppObj[Index(m_x+1,m_y)];
		if(pObj!=NULL&&pObj->IsFire()&&((CFire*)pObj)->GetFireKind()!=FIRE_L)
			m_renderKind = FIRE_CENTER;
		break;
	case FIRE_LR:
		pObj = g_ppObj[Index(m_x,m_y-1)];
		if(pObj!=NULL&&pObj->IsFire()&&((CFire*)pObj)->GetFireKind()!=FIRE_D)
			m_renderKind = FIRE_CENTER;
		pObj = g_ppObj[Index(m_x,m_y+1)];
		if(pObj!=NULL&&pObj->IsFire()&&((CFire*)pObj)->GetFireKind()!=FIRE_U)
			m_renderKind = FIRE_CENTER;
		break;
	}
	//Ç±Ç±Ç©ÇÁÉAÉjÉÅä÷åW
	m_anmCount++;
	if(m_anmCount == MAX_ANM_COUNT)
	{
		m_anmCount = 0;
		m_anmFrame++;
		if(m_anmFrame == 3)
		{
			g_ppObj[Index(m_x,m_y)]=NULL;
			delete this;
		}
	}
}
void CFire::Render()
{
	RECT rect;
	rect.left = m_anmFrame%2 * 32;
	rect.right = (m_anmFrame%2+1) * 32;
	rect.top = (int)m_renderKind * 32;
	rect.bottom = rect.top + 32;
	s_img.BltRsv(m_y*32,MAP_X+m_x*32,MAP_Y+m_y*32,&rect);
}
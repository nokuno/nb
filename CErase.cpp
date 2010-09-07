#include "stdapp.h"
#include "nb.h"
CMysImageEx CErase::s_img;
const int CErase::MAX_ANM_COUNT = 4;
const int CErase::MAX_ANM_FRAME = 4;
void CErase::LoadImage()
{
	char buffer[32];
	s_img.LoadImage(prfGetString(szIniFile,"img","erase",buffer));
	s_img.CreateMask();
}
CErase::CErase(int itemKind,int x,int y,int bombIndex)
{
	m_itemKind=itemKind;
	m_x=x;
	m_y=y;
	m_bombIndex = bombIndex;
	m_anmCount = 0;
	m_anmFrame = 0;
}
void CErase::StepFrame()
{
	m_anmCount++;
	if(m_anmCount == MAX_ANM_COUNT)
	{
		m_anmCount = 0;
		m_anmFrame++;
		if(m_anmFrame == MAX_ANM_FRAME)
		{
			if(m_itemKind != FLAG_UNINI)
				new CItem(m_itemKind,m_x,m_y);
			else
				g_ppObj[Index(m_x,m_y)]=NULL;
			delete this;
		}
	}
}
void CErase::Render()
{
	RECT rect;
	rect.left = m_anmFrame * 32;
	rect.right = (m_anmFrame+1) * 32;
	rect.top = 0;
	rect.bottom = 64;
	s_img.BltRsv(m_y*32,MAP_X+m_x*32,MAP_Y+(m_y-1)*32,&rect);
}
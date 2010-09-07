#include "stdapp.h"
#include "nb.h"
#include "CIce.h"

CMysImageEx CIce::s_img;
int CIce::Init()
{
	char buffer[32];
	s_img.LoadImage(prfGetString(szStageFile,"img","ice",buffer));
	return 1;
}
void CIce::Render()
{
	RECT rect = {0,0,32,32};
	s_img.BltRsv(-100,MAP_X+m_x*32,MAP_Y+m_y*32,&rect);
}
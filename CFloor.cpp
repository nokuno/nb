#include "stdapp.h"
#include "nb.h"
#include "CFloor.h"

CMysImageEx CFloor::s_img;
int CFloor::InitFloor()
{
	char buffer[32];
	s_img.LoadImage(prfGetString(szStageFile,"img","floor",buffer));
	return 1;
}
void CFloor::Render()
{
	RECT rect = {0,0,32,32};
	s_img.BltRsv(-100,MAP_X+m_x*32,MAP_Y+m_y*32,&rect);
}
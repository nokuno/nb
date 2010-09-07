#include "stdapp.h"
#include "nb.h"

void InitObj()
{
	g_ppObj = new CObj*[MAP_SIZE];
	for(int i=0;i<MAP_SIZE;i++)
		g_ppObj[i]=NULL;

	InitBlock();
	CBomb::LoadImage();
	CFire::LoadImage();
	CErase::LoadImage();
	CItem::LoadImage();
}
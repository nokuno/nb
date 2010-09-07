#include "stdapp.h"
#include "nb.h"
#include "CFloor.h"
#include "CIce.h"
int NewMap();
int InitMap()
{
	g_ppMap = new CMap* [MAP_SIZE];
	for(int i=0;i<MAP_SIZE;i++)
		g_ppMap[i] = NULL;
	CFloor::InitFloor();
	CIce::Init();
	NewMap();
	return 1;
}
int NewMap()
{
	int i,j;
	CBlock::m_num = prfGetInt(szStageFile,"item","block");	//�����u���b�N��
	//�v���t�@�C���m��
	char **prfArray = new char*[MAP_SIZE];	//�v���t�@�C���z��i�ꎟ���j
	for(i=0;i<MAP_SIZE;i++)
		prfArray[i] = new char[3];//�ЂƂ̃v���t�@�C���͓񕶎�+�k������
	GetObjProfile(prfArray,"map");
	for(i=0;i<MAP_SIZE_X;i++)
		for(j=0;j<MAP_SIZE_Y;j++)
		{
			if(strcmp(prfArray[Index(i,j)],"F0") == 0)
				g_ppMap[Index(i,j)] = new CFloor(i,j);
			else if(strcmp(prfArray[Index(i,j)],"I0") == 0)
				g_ppMap[Index(i,j)] = new CIce(i,j);
		}
	for(i=0;i<MAP_SIZE;i++)
		delete[] prfArray[i];
	delete[] prfArray;
	return true;
}
//nbfunc.cpp
//�O���[�o���֐�
#include "stdapp.h"
#include "nb.h"
int GetIndex(int x,int y)
{
	if(!(
		(x%2==1||y%2==1)&&
		(x!=0&&y!=0&&x!=MAP_SIZE_X-1&&y!=MAP_SIZE_Y-1)
		))
		return -1;
	if(y%2)
		return
			(y-1)/2*(MAP_SIZE_X-2) + (y-1)/2*(MAP_SIZE_X-1)/2 + x -1;
	else
		return
			y/2*(MAP_SIZE_X-2) + (y/2-1)*(MAP_SIZE_X-1)/2 + (x+1)/2 -1;
}
int Index(int x,int y)
{
	return MAP_SIZE_X*y + x;
}
void GetObjProfile(char **bufArray,const char *section)
{
	int i,j;
	char *buffer=new char[MAP_SIZE*4+1];
	char **lplpszArray;	//������z��
	lplpszArray = new char* [MAP_SIZE_Y];
	for(i=0;i<MAP_SIZE_Y;i++)
	{
		lplpszArray[i] = new char [MAP_SIZE_X*4+1];
		char szi[3];
		itoa(i,szi,10);//i�𕶎���ɂ���
		prfGetString(szStageFile,section,szi,lplpszArray[i]);
	}
	buffer[0]='\0';//buffer���󕶎��ŏ�����
	for(j=0;j<MAP_SIZE_Y;j++)
	{
		strcat(buffer,lplpszArray[j]);
	}
	for(i=0;i<MAP_SIZE_Y;i++)
		delete[] lplpszArray[i];
	delete[] lplpszArray;
	//bufArray��������
	for(i=0;i<MAP_SIZE;i++)
	{
		strcpy(bufArray[i],"��");
	}
	//��������v���t�@�C���؂���n��
	char *pbuf = buffer;
	for(i=0;i<MAP_SIZE;i++)
	{
		strdiv(bufArray[i],pbuf,',');
	}
	delete[] buffer;
}
bool CheckBombValue()
{
	int i;
	int sum=0;
	for(i=0;i<5;i++)
		sum+=g_player[i].GetBombNum();
	if(sum==0)
		for(i=0;i<MAP_SIZE;i++)
			if(g_value[i])
			{
				MessageInt(g_value[i],"error");
				MessageInt(i,"i");}
	return true;
}
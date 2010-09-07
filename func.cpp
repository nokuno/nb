#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "windows.h"
#include "func.h"

//������Ԃ�(���Ȃ�1�A���Ȃ�-1�A0�Ȃ�0)
int sign(int num)
{
	if(num == 0)
		return 0;
	else
		return num/abs(num);
}
//�l�̌ܓ�
int round(float n)
{
	if(n-0.5<(int)n)
		return (int)n;
	else
		return (int)n+1;
}
//���b�Z�[�W�{�b�N�X�ȗ���
void MessageBox(const char *lpText)
{
	MessageBox(NULL,lpText,"�Ȃ������{���o�[",MB_OK);
}
/********************************************************************************

	������^�v���t�@�C���ǂݍ���

********************************************************************************/

char *prfGetString(const char *file, const char *section, const char *key, char *buffer)
{
	char path[MAX_PATH+sizeof(file)];
	char *temp;
	GetModuleFileName(NULL, path, sizeof(path));
	temp = strrchr(path, '\\');
	if(temp){
		strcpy(temp, "\\");
		strcat(path, file);
	}
	else{
		strcat(path, "\\");
		strcat(path, file);
	}
	GetPrivateProfileString(section, key, "DEFAULT", buffer, 128, path);
	return buffer;
}
/********************************************************************************

	������^�v���t�@�C����������

********************************************************************************/

void prfSetString(const char *file, const char *section, const char *key, const char *buffer)
{
	char path[MAX_PATH+sizeof(file)];
	char *temp;
	GetModuleFileName(NULL, path, sizeof(path));
	temp = strrchr(path, '\\');
	if(temp){
		strcpy(temp, "\\");
		strcat(path, file);
	}
	else{
		strcat(path, "\\");
		strcat(path, file);
	}
	WritePrivateProfileString(section, key, buffer, path);
}
/********************************************************************************

	�����^�v���t�@�C���ǂݍ���

********************************************************************************/

int prfGetInt(const char *file, const char *section, const char *key)
{
	char path[MAX_PATH+sizeof(file)];
	char *temp;

	GetModuleFileName(NULL, path, sizeof(path));
	temp = strrchr(path, '\\');
	if(temp){
		strcpy(temp, "\\");
		strcat(path, file);
	}
	else{
		strcat(path, "\\");
		strcat(path, file);
	}	
	return GetPrivateProfileInt(section, key, 0, path);
}
/********************************************************************************

	Int�^��\������MessageBox(�f�o�b�O�p)

********************************************************************************/

void MessageInt(const int num,char *caption)
{
	char str[32];
	sprintf( str, "%d", num );
	MessageBox(NULL,str,caption,MB_OK);
}
/********************************************************************************

	Int�^��\������MessageBox(�f�o�b�O�p)

********************************************************************************/

void OutputDebugInt(const int num)
{
	char str[32];
	sprintf( str, "%d", num );
	OutputDebugString(str);
}
void OutputDebugInt(const int num,const char* cap)
{
	char str[128];
	sprintf( str, "%s:%d\n", cap, num );
	OutputDebugString(str);
}
/********************************************************************************

	��������u,�v�Ȃǂŋ�؂��Đ����l��Ԃ��A�����񎩐g�́u,�v�̌��̂ݎc��
	�����_�̕����񓯎m�̈����Z(buffer=string-c)�ɓ�����B

********************************************************************************/

char *strdiv(char *buffer,char *&string,int c,const char *def)
{
	char *temp;
	
	if((temp = strchr(string, c))==NULL){
		if((temp = strchr(string,NULL))==NULL){//�s����NULL�������ꍇ�i�܂����̎��̗�O����
			strcpy( buffer,def);
			return buffer;
		}
	}
	if(temp-string == 0)//�J���}�A��������ѕ�����I�[�ȍ~
		strcpy( buffer,def);
	else{//����ȊO
		strncpy(buffer,string,temp-string);
		buffer[temp-string]=0;
	}
	string = temp;
	string++;
	return buffer;
}

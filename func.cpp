#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "windows.h"
#include "func.h"

//符号を返す(正なら1、負なら-1、0なら0)
int sign(int num)
{
	if(num == 0)
		return 0;
	else
		return num/abs(num);
}
//四捨五入
int round(float n)
{
	if(n-0.5<(int)n)
		return (int)n;
	else
		return (int)n+1;
}
//メッセージボックス簡略版
void MessageBox(const char *lpText)
{
	MessageBox(NULL,lpText,"なえちゃんボンバー",MB_OK);
}
/********************************************************************************

	文字列型プロファイル読み込み

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

	文字列型プロファイル書き込み

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

	整数型プロファイル読み込み

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

	Int型を表示するMessageBox(デバッグ用)

********************************************************************************/

void MessageInt(const int num,char *caption)
{
	char str[32];
	sprintf( str, "%d", num );
	MessageBox(NULL,str,caption,MB_OK);
}
/********************************************************************************

	Int型を表示するMessageBox(デバッグ用)

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

	文字列を「,」などで区切って整数値を返し、文字列自身は「,」の後ろのみ残る
	ラムダの文字列同士の引き算(buffer=string-c)に当たる。

********************************************************************************/

char *strdiv(char *buffer,char *&string,int c,const char *def)
{
	char *temp;
	
	if((temp = strchr(string, c))==NULL){
		if((temp = strchr(string,NULL))==NULL){//行末にNULLが無い場合（まさかの時の例外処理
			strcpy( buffer,def);
			return buffer;
		}
	}
	if(temp-string == 0)//カンマ連続時および文字列終端以降
		strcpy( buffer,def);
	else{//それ以外
		strncpy(buffer,string,temp-string);
		buffer[temp-string]=0;
	}
	string = temp;
	string++;
	return buffer;
}

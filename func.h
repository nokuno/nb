//func.h
#ifndef _INC_FUNC_H_
#define _INC_FUNC_H_

char *prfGetString(const char *file, const char *section, const char *key, char *buffer);
int prfGetInt(const char *file, const char *section, const char *key);
void prfSetString(const char *file, const char *section, const char *key, const char *buffer);
void MessageInt(const int num,char *caption = "");
void OutputDebugInt(const int num);
void OutputDebugInt(const int num,const char* cap);
char *strdiv(char *buffer,char *&string,int c,const char *def="DEFAULT");
int sign(int num);
int GetIndex(int x,int y);
int Index(int x,int y);
void EndGame();
int round(float n);
bool CheckBombValue();
void GetObjProfile(char **bufArray,const char *section);
void MessageBox(const char *lpText);
#endif
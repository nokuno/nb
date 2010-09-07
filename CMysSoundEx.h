//
// CMysSoundEx.h
// サウンド管理クラス
//

#pragma once

#include <list>
using namespace std;

#include "Mys\\CMysSound.h"

class CMysSoundEx
{
public:
	CMysSoundEx(){};
	~CMysSoundEx(){};
	bool LoadSound(char* szFileName,BYTE mode = MYS_SOUND_NORMAL);
	void Play();
	static void DeleteAll();
private:
	static list<CMysSound*> s_lSound;
	CMysSound *m_snd;
};
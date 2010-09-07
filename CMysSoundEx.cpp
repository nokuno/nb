#include "CMysSoundEx.h"
list<CMysSound*> CMysSoundEx::s_lSound;
bool CMysSoundEx::LoadSound(char* szFileName,BYTE mode)
{
	m_snd = new CMysSound;
	bool ret = m_snd->LoadSound(szFileName,mode);
	s_lSound.push_back(m_snd);
	return ret;
}
void CMysSoundEx::DeleteAll()
{
	list<CMysSound*>::iterator itr;
	for(itr = s_lSound.begin();itr!=s_lSound.end();++itr)
		delete *itr;
	s_lSound.clear();
}
void CMysSoundEx::Play()
{
#ifndef _DEBUG
	m_snd->Play();
#endif
}
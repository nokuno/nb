#include "CMysMidiEx.h"
void CMysMidiEx::Play()
{
#ifndef _DEBUG
	CMysMidi::Play();
#endif
}
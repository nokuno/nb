
#include "stdapp.h"

#include "CMysInput2.h"

void CMysInput2::GetKeyState(byte pKeyState[])
{
	//初期化されていなかったら初期化する
	if(m_pDIKeyboard==NULL)	RestoreDevice();

	HRESULT hr=0;
	//キーボード状態取得
	hr = m_pDIKeyboard->GetDeviceState(256,pKeyState);
	if(hr==DI_OK)
	{
	}
	else if(hr==DIERR_INPUTLOST || hr==DIERR_NOTACQUIRED)
	{
		//入力デバイスのアクセス権を失っているので再取得したのちリトライ
		m_pDIKeyboard->Acquire();
		m_pDIKeyboard->GetDeviceState(256,pKeyState);
	}
	else
	{
		//予想外のときには初期化しなおす
		RestoreDevice();
	}
}

void CMysInput2::GetJoyState(dword index, byte pDirState[/*4*/], byte pBtnState[/*32*/])
{
	if(index>=m_nJoynum) return;

	HRESULT hr=0;
	unsigned int i;
	BYTE diKeyState[256]={0};
	DIJOYSTATE diJoyState;

	//ジョイパッドをチェック
	ZeroMemory(&diJoyState,sizeof(DIJOYSTATE));
	m_ppDIJoystick[index]->Poll();
	//ジョイパッド状態取得
	if(m_ppDIJoystick[index]->GetDeviceState(sizeof(DIJOYSTATE),&diJoyState)==DIERR_INPUTLOST)
	{
		//入力デバイスのアクセス権を失っているので再取得したのちリトライ
		m_ppDIJoystick[index]->Acquire();
		m_ppDIJoystick[index]->Poll();
		m_ppDIJoystick[index]->GetDeviceState(sizeof(DIJOYSTATE),&diJoyState);
	}
	//入力チェック
	if(diJoyState.lX <= -50) pDirState[0] = 1;
	else pDirState[0] = 0;
	if(diJoyState.lX >= 50) pDirState[1] = 1;
	else pDirState[1] = 0;
	if(diJoyState.lY <= -50) pDirState[2] = 1;
	else pDirState[2] = 0;
	if(diJoyState.lY >= 50) pDirState[3] = 1;
	else pDirState[3] = 0;

	for(i=0;i<32;i++) {
		if(diJoyState.rgbButtons[i]) pBtnState[i] = 1;
		else pBtnState[i] = 0;
	}
}
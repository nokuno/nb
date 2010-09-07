
#include "stdapp.h"

#include "CMysInput2.h"

void CMysInput2::GetKeyState(byte pKeyState[])
{
	//����������Ă��Ȃ������珉��������
	if(m_pDIKeyboard==NULL)	RestoreDevice();

	HRESULT hr=0;
	//�L�[�{�[�h��Ԏ擾
	hr = m_pDIKeyboard->GetDeviceState(256,pKeyState);
	if(hr==DI_OK)
	{
	}
	else if(hr==DIERR_INPUTLOST || hr==DIERR_NOTACQUIRED)
	{
		//���̓f�o�C�X�̃A�N�Z�X���������Ă���̂ōĎ擾�����̂����g���C
		m_pDIKeyboard->Acquire();
		m_pDIKeyboard->GetDeviceState(256,pKeyState);
	}
	else
	{
		//�\�z�O�̂Ƃ��ɂ͏��������Ȃ���
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

	//�W���C�p�b�h���`�F�b�N
	ZeroMemory(&diJoyState,sizeof(DIJOYSTATE));
	m_ppDIJoystick[index]->Poll();
	//�W���C�p�b�h��Ԏ擾
	if(m_ppDIJoystick[index]->GetDeviceState(sizeof(DIJOYSTATE),&diJoyState)==DIERR_INPUTLOST)
	{
		//���̓f�o�C�X�̃A�N�Z�X���������Ă���̂ōĎ擾�����̂����g���C
		m_ppDIJoystick[index]->Acquire();
		m_ppDIJoystick[index]->Poll();
		m_ppDIJoystick[index]->GetDeviceState(sizeof(DIJOYSTATE),&diJoyState);
	}
	//���̓`�F�b�N
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
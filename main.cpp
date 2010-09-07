/*-----------------------------------------------------
	
	MysLib�T���v��

    main.cpp

------------------------------------------------------*/

#include "stdapp.h"
#include "resource.h"
#include "nb.h"
//LRESULT CALLBACK StartDlgProc(HWND, UINT, WPARAM, LPARAM);

GApp gapp;
CMysLib2* g_pcMysLib = NULL;			//�B���CMysLib2�I�u�W�F�N�g(extern���Ďg��)
CMysInput2* g_pcMysInput = NULL;		//�B���CMysInput�I�u�W�F�N�g(extern���Ďg��)

CMysMidiDev* g_pcMysMidiDev = NULL;	//�A�N�e�B�u���ǂ�����ݒ肷�邽�߂����Ɏg�p(extern���Ďg��)

MSG g_msg;

void MysStart();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR,int)
{
	//�O���[�o���ϐ�������
	ZeroMemory(&gapp,sizeof(GApp));
	gapp.hInstance=hInstance;
	gapp.lpszTitle = (char*)APPNAME;
	ZeroMemory(&g_msg,sizeof(MSG));

	//MYS���C�u�����̏�����
	g_pcMysLib = new CMysLib2;
	if(g_pcMysLib==NULL) goto ENDAPP;
	if(!g_pcMysLib->InitApp(hInstance)) goto ENDAPP;

	//Sound��MIDI�̃N���X��������ꂽ��ł����̏I�������Ȃ���΂Ȃ�Ȃ��̂�
	//CSmp�N���X�����ł͂�炸�A�����ł�邱��

	//�T�E���h�@�\������
	if(!mysInitSound()) goto ENDAPP;
	//MIDI�@�\�̏�����
	g_pcMysMidiDev = new CMysMidiDev;
	if(!g_pcMysMidiDev->Initialize()) goto ENDAPP;

	//���͋@�\������
	g_pcMysInput = new CMysInput2;
	g_pcMysInput->Initialize("sample.cfg");

	MysStart();

	gapp.hWnd = NULL;

	ENDAPP:

	delete g_pcMysInput;
	delete g_pcMysLib;

	DELETE_PTR(g_pcMysMidiDev);
	mysEndSound();

	return g_msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;


	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		HDC hSrc;
		gapp.pBacksurf->GetDC(&hSrc);
		BitBlt(hdc,0, 0, gapp.dwDispSizeX, gapp.dwDispSizeY, hSrc,0,0,SRCCOPY);
		gapp.pBacksurf->ReleaseDC(hSrc);
		EndPaint(hWnd,&ps);
		break;
	case WM_ACTIVATE:
		if(LOWORD(wParam)==WA_ACTIVE || LOWORD(wParam)==WA_CLICKACTIVE)
		{
			g_pcMysLib->SetActive(true);
			if(g_pcMysMidiDev) g_pcMysMidiDev->SetActive(true);
			//g_pcMysLib->RestorePrimSurf(); //��������
		}
		else if(LOWORD(wParam)==WA_INACTIVE)
		{
			g_pcMysLib->SetActive(false);
			if(g_pcMysMidiDev) g_pcMysMidiDev->SetActive(false);
		}
		break;

	case WM_SIZE:
		g_pcMysLib->WmSize(wParam,lParam);
		break;

	case WM_MOVE:
		g_pcMysLib->WmMove(wParam,lParam);
		break;

	case WM_KEYDOWN:

		switch(wParam)
		{
		case VK_F1:
			g_pcMysLib->ChangeDisplayMode();
			break;
		case VK_ESCAPE:
			PostMessage(hWnd,WM_CLOSE,0,0);
			break;
		}
		break;

	case WM_SYSKEYDOWN:
		switch(wParam)
		{
		case VK_RETURN:
			if(lParam&0x20000000) g_pcMysLib->ChangeDisplayMode();
			break;
		}

	case WM_SYSCOMMAND:
		g_pcMysLib->WmSysCommand(wParam,lParam);
		DefWindowProc(hWnd, msg, wParam, lParam);
		break;


	case WM_CLOSE:

		if(MysMessageBox("�Q�[�����I�����܂����H",gapp.lpszTitle,MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			g_pcMysLib->EndApp();
			DestroyWindow(hWnd);
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:          // ��������Ă��Ȃ��ꍇ�ɂ̓��b�Z�[�W��n���܂�
	    return (DefWindowProc(hWnd, msg, wParam, lParam));
	}

    return (0L);
}


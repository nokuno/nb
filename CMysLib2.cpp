
#include "stdapp.h"
#include "CMysLib2.h"
#include "resource.h"

#define SYSTEMINI_FILENAME "system.dat"
//"startDialog.cpp"�ɂ����l�̒�`������̂Œ���

extern GApp gapp;

LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wParam, LPARAM lParam);
HRESULT WINAPI EnumModesCallback2(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);

bool CMysLib2::InitApp(HINSTANCE hInstance)
{
	m_dwStyleWindowed &= ~WS_MAXIMIZEBOX;
	ReadSysIni();

	WNDCLASS wc;

	// WNDCLASS�\���̂̏�����
	wc.style                = CS_BYTEALIGNCLIENT|CS_VREDRAW|CS_HREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hCursor              = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground        = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName         = NULL;
	wc.lpszClassName        = gapp.lpszTitle;

	try
	{
		// �E�B���h�E�N���X��o�^
		if(RegisterClass(&wc) == 0)	throw "�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B";

		// �E�B���h�E���쐬
		gapp.hWnd = CreateWindowEx(0,gapp.lpszTitle,gapp.lpszTitle,m_dwStyleWindowed,
				(int)m_sWindowPosX,(int)m_sWindowPosY,
				(int)(gapp.dwDispSizeX),(int)(gapp.dwDispSizeY),NULL,NULL,hInstance,NULL);

		if(gapp.hWnd == NULL) throw "�E�B���h�E�̍쐬�Ɏ��s���܂����B";
 

		//DirectDraw������(CreateDirectDraw����EMULATIONONLY�͎��s���邱�Ƃ�����)

		/*
		if (FAILED(CoInitialize(NULL))) throw "COM�̏������Ɏ��s���܂����B";

		HRESULT ddrval = CoCreateInstance(CLSID_DirectDraw,
			NULL, CLSCTX_ALL, IID_IDirectDraw7, (void**)&(gapp.pDDraw));

		if(!FAILED(ddrval)) {
			if(m_systemini.bWaitVBI) ddrval = IDirectDraw7_Initialize(gapp.pDDraw,NULL);
			else ddrval = IDirectDraw7_Initialize(gapp.pDDraw,(GUID*)DDCREATE_EMULATIONONLY);
		}

		if(ddrval!=DD_OK) {
			if(m_systemini.bWaitVBI) ddrval = IDirectDraw7_Initialize(gapp.pDDraw,(GUID*)DDCREATE_EMULATIONONLY);
			else ddrval = IDirectDraw7_Initialize(gapp.pDDraw,NULL);
		}
		if(ddrval!=DD_OK) {
			throw "DirectDraw�̍쐬�Ɏ��s���܂����B";
		}
		*/

		//EMULATIONONLY�ɂ͂��Ȃ�
		HRESULT hr = DirectDrawCreateEx(NULL,(void**)&gapp.pDDraw,IID_IDirectDraw7,NULL);
		if(hr != DD_OK) throw "DirectDraw�̍쐬�Ɏ��s���܂����B";

		//�N���b�p�̍쐬
		gapp.pDDraw->CreateClipper(0,&m_pClipper,NULL);	//�G���[�`�F�b�N�Ȃ�
		m_pClipper->SetHWnd(0,gapp.hWnd);

		//�����Ńv���C�}���T�[�t�F�C�X�������
		if(m_systemini.bFullScreen==0) SetFullScreen(false,false);
		else SetFullScreen(true,false);


		if(m_systemini.bMaximize) ShowWindow(gapp.hWnd,SW_MAXIMIZE);


		//�s�N�Z���t�H�[�}�b�g�`�F�b�N

		DDPIXELFORMAT ddPrimPixfmt;
		ZeroMemory(&ddPrimPixfmt,sizeof(DDPIXELFORMAT));
		ddPrimPixfmt.dwSize = sizeof(DDPIXELFORMAT);
		if(gapp.pPrimsurf->GetPixelFormat(&ddPrimPixfmt) != DD_OK)
			throw "�s�N�Z���t�H�[�}�b�g�̎擾�Ɏ��s���܂����B";

		m_bBitCount = (BYTE)(ddPrimPixfmt.dwRGBBitCount);

		if(m_bBitCount==8)
			;
		else if(m_bBitCount==16)
		{
			if(ddPrimPixfmt.dwRBitMask == 0x0000F800) m_bPixfmt = true;  //565
			else if(ddPrimPixfmt.dwRBitMask== 0x00007C00)m_bPixfmt = false;  //555
			else  throw "�Ή����Ă��Ȃ��s�N�Z���t�H�[�}�b�g�ł��B";
		}
		else if(m_bBitCount==24)
		{
			if(ddPrimPixfmt.dwRBitMask == 0x00FF0000) m_bPixfmt = true;
			else if(ddPrimPixfmt.dwRBitMask == 0x000000FF) m_bPixfmt = false;
			else  throw "�Ή����Ă��Ȃ��s�N�Z���t�H�[�}�b�g�ł��B";
		}
		else if(m_bBitCount==32)
		{
			if(ddPrimPixfmt.dwRBitMask == 0x00FF0000) m_bPixfmt = true;
			else if(ddPrimPixfmt.dwRBitMask == 0x000000FF) m_bPixfmt = false;
			else  throw "�Ή����Ă��Ȃ��s�N�Z���t�H�[�}�b�g�ł��B";
		}
		else throw "�Ή����Ă��Ȃ��s�N�Z���t�H�[�}�b�g�ł��B";


		//�o�b�N�o�b�t�@���쐬
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth = gapp.dwDispSizeX;
		ddsd.dwHeight = gapp.dwDispSizeY;

		if(m_bBitCount==16)
		{
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			if(gapp.pDDraw->CreateSurface(&ddsd,&gapp.pBacksurf,NULL)!=DD_OK)
				throw "�p�b�N�T�[�t�F�C�X�̍쐬�Ɏ��s���܂����B";

			gapp.bPft565 = m_bPixfmt;
		}
		else
		{
			DDSURFACEDESC2 ddsdEnum;
			ZeroMemory(&ddsdEnum, sizeof(DDSURFACEDESC2));
			ddsdEnum.dwSize = sizeof(DDSURFACEDESC2);
			ddsdEnum.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			ddsdEnum.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_SYSTEMMEMORY;
			ddsdEnum.dwWidth = gapp.dwDispSizeX;
			ddsdEnum.dwHeight = gapp.dwDispSizeY;

			if(gapp.pDDraw->EnumDisplayModes(0,&ddsdEnum,&(gapp.bPft565),EnumModesCallback2)!=DD_OK)
				throw "�f�B�X�v���C���[�h�̗񋓂Ɏ��s���܂����B";


			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			DDPIXELFORMAT ddPixfmt;
			ZeroMemory(&ddPixfmt,sizeof(DDPIXELFORMAT));
			ddPixfmt.dwSize = sizeof(DDPIXELFORMAT);
			ddPixfmt.dwFlags = DDPF_RGB;
			ddPixfmt.dwRGBBitCount = 16;

			if(gapp.bPft565)
			{
				ddPixfmt.dwRBitMask = 0x0000F800;
				ddPixfmt.dwGBitMask = 0x000007E0;
				ddPixfmt.dwBBitMask = 0x0000001F;
			}
			else
			{
				ddPixfmt.dwRBitMask = 0x00007C00;
				ddPixfmt.dwGBitMask = 0x000003E0;
				ddPixfmt.dwBBitMask = 0x0000001F;
			}

			ddsd.ddpfPixelFormat = ddPixfmt;

			if(gapp.pDDraw->CreateSurface(&ddsd,&gapp.pBacksurf,NULL)!=DD_OK)
				throw "�p�b�N�T�[�t�F�C�X�̍쐬�Ɏ��s���܂����B";
		}

		//�o�b�N�T�[�t�F�C�X�C���[�W�ւ̃|�C���^�擾
		ZeroMemory(&ddsd,sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		if(gapp.pBacksurf->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL)!=DD_OK) throw "�T�[�t�F�C�X�̎擾�Ɏ��s���܂����B";
		gapp.pBackImage = ddsd.lpSurface;
		gapp.lBackPitch = ddsd.lPitch;
		gapp.pBacksurf->Unlock(NULL);

	}
	catch(char* err)
	{
		MysMessageBox(err,NULL,MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

void CMysLib2::EndApp()
{
	WriteSysIni();
}

bool CMysLib2::ReadSysIni()
{
	//�E�B���h�E���Ȃ�(hwnd=NULL)�ꍇ������̂Œ���

	char* szFullName=NULL;
	char szGet[40];

	DWORD dwLength = GetFullPathName(SYSTEMINI_FILENAME,0,NULL,NULL);
	szFullName = new char[dwLength];
	if(szFullName==NULL) return false;

	GetFullPathName(SYSTEMINI_FILENAME,dwLength,szFullName,NULL);

	GetPrivateProfileString("MAIN","fullscreen","true",szGet,40,szFullName);
	if(strcmp(szGet,"false")==0) m_systemini.bFullScreen = 0;
	else m_systemini.bFullScreen = 1;

	int dispW = GetSystemMetrics(SM_CXSCREEN);
	int dispH = GetSystemMetrics(SM_CYSCREEN);

	m_systemini.sWindowPosX = 
		(short)GetPrivateProfileInt("MAIN","pos_x",(dispW-gapp.dwDispSizeX)/2-3,szFullName);
	m_systemini.sWindowPosY = 
		(short)GetPrivateProfileInt("MAIN","pos_y",(dispH-gapp.dwDispSizeY)/2-38,szFullName);

	m_sWindowPosX = m_systemini.sWindowPosX;
	m_sWindowPosY = m_systemini.sWindowPosY;

	GetPrivateProfileString("MAIN","maximize","false",szGet,40,szFullName);
	if(strcmp(szGet,"false")==0) m_systemini.bMaximize = 0;
	else m_systemini.bMaximize = 1;

	GetPrivateProfileString("MAIN","waitvbi","true",szGet,40,szFullName);
	if(strcmp(szGet,"false")==0) m_systemini.bWaitVBI = 0;
	else m_systemini.bWaitVBI = 1;

	return true;
}

bool CMysLib2::WriteSysIni()
{
	//�E�B���h�E���󂳂��O�ɌĂяo�����Ɓi��m_sWindowPosX=m_sWindowPosY=0�ɂȂ��Ă��܂�

	//�ʂɕK�v�Ȃ�
	m_systemini.bFullScreen = gapp.bFullScreen;
	m_systemini.sWindowPosX = m_sWindowPosX;
	m_systemini.sWindowPosY = m_sWindowPosY;
	m_systemini.bMaximize = m_bMaximize;

	char* szFullName=NULL;
	char szSet[40];

	DWORD dwLength = GetFullPathName(SYSTEMINI_FILENAME,0,NULL,NULL);
	szFullName = new char[dwLength];
	if(szFullName==NULL) return false;

	GetFullPathName(SYSTEMINI_FILENAME,dwLength,szFullName,NULL);

	if(gapp.bFullScreen) strcpy(szSet,"true");
	else strcpy(szSet,"false");
	WritePrivateProfileString("MAIN","fullscreen",szSet,szFullName);

	sprintf(szSet,"%d",m_systemini.sWindowPosX);
	WritePrivateProfileString("MAIN","pos_x",szSet,szFullName);

	sprintf(szSet,"%d",m_systemini.sWindowPosY);
	WritePrivateProfileString("MAIN","pos_y",szSet,szFullName);

	if(m_bMaximize) strcpy(szSet,"true");
	else strcpy(szSet,"false");
	WritePrivateProfileString("MAIN","maximize",szSet,szFullName);
	
//	if(m_systemini.bWaitVBI) strcpy(szSet,"true");
//	else strcpy(szSet,"false");
//	WritePrivateProfileString("MAIN","waitvbi",szSet,szFullName);

	return true;
}
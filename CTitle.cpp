
#include "stdapp.h"
#include "nb.h"
#include "process.h"

CTitle*			pcActive;//�A�N�e�B�u�ȃN���X
CTitle*			pcTitle;
CSelectPlayer*	pcSelectPlayer;
CSelectStage*	pcSelectStage;
CShowResult*	pcShowResult;
CMysImageEx CTitle::s_imgBG;
CMysImageEx CTitle::s_imgStrs;
CMysImageEx CTitle::s_imgTitle;

void MysStart()
{
	bool stage = false;
	while(StartTitle(stage))
	{
		if(!StartMainGame()) break;
		stage = true;
	}
	return;
}
int StartTitle(bool stage)
{
	pcTitle = new CTitle;
	pcSelectPlayer = new CSelectPlayer;
	pcSelectStage = new CSelectStage;
	if(stage)
		pcShowResult = new CShowResult;
	g_pMidi = NULL;
	if(pcTitle==NULL||pcSelectPlayer==NULL||pcSelectPlayer==NULL) return 0;
	if(!stage)
		pcActive = pcTitle;//�A�N�e�B�u���^�C�g���N���X�ɂ���
	else
		pcActive = pcShowResult;

	//�����A����������郂�[�h���ǂ������`�F�b�N���A����ɉ����đ҂��Ԃ�ݒ肷��
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	int waittime;	//���t���[���܂ő҂���
	if(sysini.bWaitVBI) waittime = 1000/40;	//�኱���߂ɂƂ�
	else waittime = 1000/30;
	//�߂�l
	bool ret = false;
	//�^�C�g�����[�v
	while(true)
	{
		if(sysini.bWaitVBI) pcActive->Render();
		if(!pcActive->StepFrame()) { ret=pcActive->Return(); break; }
		if(!sysini.bWaitVBI) pcActive->Render();
		if(!g_pcMysLib->WaitTime(waittime,true,true)) {ret=false;break;}
	}

	delete pcTitle;
	delete pcSelectPlayer;
	delete pcSelectStage;
	if(stage)
		delete pcShowResult;
#ifndef _DEBUG
	if(g_pMidi != NULL)
	{
		g_pMidi->Stop();
		g_pMidi->Delete();
		delete g_pMidi;
	}
#endif
	return ret;
}
bool CTitle::StepFrame()
{
	GetDevEvent();//�L�[��ԍX�V
	if(devevent[PL_1][DEV_U]==1)
		OnUp();
	if(devevent[PL_1][DEV_D]==1)
		OnDown();
	if(devevent[PL_1][DEV_B1]==1)
	{
		OnOk();
		if(m_cursor == 2)
			return false;
	}
	return true;
}
//������
bool CTitle::Initialize()
{
	for(int i=0;i<5;i++)
		for(int j=0;j<DEV_MAX;j++)
			devstate[i][j]=0;
	if(!LoadConfig())//�L�[�R���t�B�O�ǂݍ���
	{
		MessageBox(NULL,"error","config",MB_OK);
		SetDefaultConfig();
	}
	char buffer[32];
	s_imgTitle.LoadImage(prfGetString(szIniFile,"img","title",buffer));
	s_imgBG.LoadImage(prfGetString(szIniFile,"img","bg",buffer));
	s_imgStrs.LoadImage(prfGetString(szIniFile,"img","strs",buffer));
	s_imgStrs.CreateMask();
	m_cursor = 0;
	return true;
}
void CTitle::OnUp()
{
	if(m_cursor>0)
		m_cursor--;
}
void CTitle::OnDown()
{
	if(m_cursor<2)
		m_cursor++;
}
extern GApp gapp;
void CTitle::OnOk()
{
	CMysImageEx::ClearBuffer();
	char buffer[32];
	switch(m_cursor)
	{
	case 0://����
		pcActive = pcSelectPlayer;//�A�N�e�B�u��ύX
		g_pMidi = new CMysMidiEx;
		g_pMidi->LoadMidi(prfGetString(szIniFile,"bgm","menu",buffer));
		g_pMidi->SetLoop(true,4);
		g_pMidi->Play();
		break;
	case 1://�L�[�R���t�B�O
		StartConfig(gapp.hWnd,g_pcMysInput);
		if(!LoadConfig())
		{
			MessageBox(NULL,"error","config",MB_OK);
			SetDefaultConfig();
		}
		GetDevState();
		break;
	}
}

#define CURSOR_X 10
#define TITLE_STR_X 50
#define TITLE_STR_Y 300
#define TITLE_STR_W 48
void CTitle::Render()
{
	SYSTEMINI sysini = g_pcMysLib->GetSystemIni();
	if(sysini.bWaitVBI) g_pcMysLib->Render();		//VBI�҂��̓s�����ɂ��

	s_imgTitle.mysBlt(NULL,0,0);
	RECT r1 = {96,192,288,336};
	s_imgStrs.mysBlt(NULL,TITLE_STR_X,TITLE_STR_Y,&r1);//title strings
	RECT r2 = {96,368,144,416};
	s_imgStrs.mysBlt(NULL,CURSOR_X,TITLE_STR_Y+TITLE_STR_W*m_cursor,&r2);//cursor

	if(!sysini.bWaitVBI) g_pcMysLib->Render();
}
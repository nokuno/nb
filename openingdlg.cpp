#include "stdapp.h"
#include "resource.h"
#include <commctrl.h>
#include <windowsx.h>

extern CMysInput2* g_pcMysInput;
char debugstr[MAX_PATH];

LRESULT CALLBACK StartDlgProc(HWND hDlg, UINT hInst, WPARAM wparam, LPARAM lparam)
{
//�����F�R�[���o�b�N�֐��͉����������Ȃ������Ƃ�0��Ԃ��A
//�@�@�@���炩�̏����������Ƃ��͔�0(TRUE)��Ԃ��񑩂���B
//	LPCTSTR ListComb_Str[BLOODTYPE_NUM] = {LIST_COMB_STR};
	WORD w=0;
	dword joynum=0;
	static int nNo;
	char ss[MAX_PATH+10]="PAD";
	switch(hInst)
	{
	case WM_INITDIALOG:
		OutputDebugString("�_�C�A���O�{�b�N�X�������J�n...\n");
		HWND hTab, hCtrl;
		TCITEM ti;
		
		hTab = GetDlgItem(hDlg, IDC_TAB);

		//ZeroMemory(&ti,sizeof(TCITEM));
		ti.mask = TCIF_TEXT;
		ti.pszText = "�ݒ�P";
		TabCtrl_InsertItem(hTab, 0, &ti);
		ti.pszText = "�ݒ�Q";
		TabCtrl_InsertItem(hTab, 1, &ti);
		ti.pszText = "�ݒ�R";
		TabCtrl_InsertItem(hTab, 2, &ti);
		ti.pszText = "�ݒ�S";
		TabCtrl_InsertItem(hTab, 3, &ti);
		ti.pszText = "�ݒ�T";
		TabCtrl_InsertItem(hTab, 4, &ti);
		ti.pszText = "�ݒ�U";
		TabCtrl_InsertItem(hTab, 5, &ti);
		ti.pszText = "�ݒ�V";
		TabCtrl_InsertItem(hTab, 6, &ti);
		ti.pszText = "�ݒ�W";
		TabCtrl_InsertItem(hTab, 7, &ti);

//�p�b�h���\��
//���������R�[�h��
/*	hCtrl = GetDlgItem(hDlg,IDC_LIST1);
	char snum[10];
	for(w=0;w<4;w++)
	{
		//ZeroMemory(ss,sizeof(char)*(MAX_PATH+10));
		strcpy(ss,"PAD");

		//ZeroMemory(snum,sizeof(char)*10);
		sprintf(snum,"%d",w+1);
		strcat(ss,snum);
		strcat(ss," : ");
		strcat(ss,m_pJoydev[w].tszJoyname);

		ListBox_InsertString(hCtrl, w, ss);
	}
*/

//�L�[�R���t�B�O�I��
		hCtrl = GetDlgItem(hDlg,IDC_COMBO1);
		ComboBox_InsertString(hCtrl,0,"�ݒ�P");
		ComboBox_InsertString(hCtrl,1,"�ݒ�Q");
		ComboBox_InsertString(hCtrl,2,"�ݒ�R");
		ComboBox_InsertString(hCtrl,3,"�ݒ�S");
		ComboBox_InsertString(hCtrl,4,"�ݒ�T");
		ComboBox_InsertString(hCtrl,5,"�ݒ�U");
		ComboBox_InsertString(hCtrl,6,"�ݒ�V");
		ComboBox_InsertString(hCtrl,7,"�ݒ�W");
		ComboBox_SetCurSel(hCtrl, 0);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO2);
		ComboBox_InsertString(hCtrl,0,"�ݒ�P");
		ComboBox_InsertString(hCtrl,1,"�ݒ�Q");
		ComboBox_InsertString(hCtrl,2,"�ݒ�R");
		ComboBox_InsertString(hCtrl,3,"�ݒ�S");
		ComboBox_InsertString(hCtrl,4,"�ݒ�T");
		ComboBox_InsertString(hCtrl,5,"�ݒ�U");
		ComboBox_InsertString(hCtrl,6,"�ݒ�V");
		ComboBox_InsertString(hCtrl,7,"�ݒ�W");
		ComboBox_SetCurSel(hCtrl, 1);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO3);
		ComboBox_InsertString(hCtrl,0,"�ݒ�P");
		ComboBox_InsertString(hCtrl,1,"�ݒ�Q");
		ComboBox_InsertString(hCtrl,2,"�ݒ�R");
		ComboBox_InsertString(hCtrl,3,"�ݒ�S");
		ComboBox_InsertString(hCtrl,4,"�ݒ�T");
		ComboBox_InsertString(hCtrl,5,"�ݒ�U");
		ComboBox_InsertString(hCtrl,6,"�ݒ�V");
		ComboBox_InsertString(hCtrl,7,"�ݒ�W");
		ComboBox_SetCurSel(hCtrl, 2);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO4);
		ComboBox_InsertString(hCtrl,0,"�ݒ�P");
		ComboBox_InsertString(hCtrl,1,"�ݒ�Q");
		ComboBox_InsertString(hCtrl,2,"�ݒ�R");
		ComboBox_InsertString(hCtrl,3,"�ݒ�S");
		ComboBox_InsertString(hCtrl,4,"�ݒ�T");
		ComboBox_InsertString(hCtrl,5,"�ݒ�U");
		ComboBox_InsertString(hCtrl,6,"�ݒ�V");
		ComboBox_InsertString(hCtrl,7,"�ݒ�W");
		ComboBox_SetCurSel(hCtrl, 3);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO5);
		ComboBox_InsertString(hCtrl,0,"�ݒ�P");
		ComboBox_InsertString(hCtrl,1,"�ݒ�Q");
		ComboBox_InsertString(hCtrl,2,"�ݒ�R");
		ComboBox_InsertString(hCtrl,3,"�ݒ�S");
		ComboBox_InsertString(hCtrl,4,"�ݒ�T");
		ComboBox_InsertString(hCtrl,5,"�ݒ�U");
		ComboBox_InsertString(hCtrl,6,"�ݒ�V");
		ComboBox_InsertString(hCtrl,7,"�ݒ�W");
		ComboBox_SetCurSel(hCtrl, 4);

		//LoadCfg();

		//SetTimer��WM_TIMER���߂��󂯎��
		SetTimer(hDlg,100,10,NULL);

		//ComboBox_SetCurSel(GetDlgItem(m_hDlg,IDC_COMBO1), m_cAtvset[0]);
		//ComboBox_SetCurSel(GetDlgItem(m_hDlg,IDC_COMBO2), m_cAtvset[1]);
		OutputDebugString("�_�C�A���O�{�b�N�X����������\n");
	break;

	case WM_TIMER:
//		joynum = g_pcMysInput->GetJoyNum();
//		g_pcMysInput->GetKeyState(pKeyState);
//		g_pcMysInput->GetJoyState(joycfg[PL_1][JOYNUM], pDirState, pBtnState);

	break;

	case WM_COMMAND://�Ȃ�炩�̓��삪�_�C�A���O�{�b�N�X�ɂ��ꂽ
		switch(wparam)
		{//��̓I�ȃR�}���h
		case IDOK://�N��
			OutputDebugString("�n�j�I��\n");
			EndDialog(hDlg,IDOK);//��Ԗڂ̈�����DialogBox�֐��̖߂�l����B
			return TRUE;
		break;
		case IDCANCEL://�I��
			OutputDebugString("�L�����Z���I��\n");
			EndDialog(hDlg,IDCANCEL);//��Ԗڂ̈�����DialogBox�֐��̖߂�l����B
			return TRUE;
		break;
		default:
		break;
		}
	break;

	case WM_NOTIFY:
		switch (((NMHDR *)lparam)->code)
		{
		case TCN_SELCHANGE:
			OutputDebugString("�^�u�؂�ւ� ");
			hTab = GetDlgItem(hDlg, IDC_TAB);
			nNo = TabCtrl_GetCurSel(hTab);
			OutputDebugString("�^�uno�� ");
			OutputDebugString(_itoa(nNo,debugstr,10));
			OutputDebugString("\n");
			//ChangeTab(nNo);
			{	//�^�u�؂�ւ����Ƃ�EDIT_UP�`EDIT_B4�܂ł�
				//nNo�̐ݒ���o��
			}
			return TRUE;
		}
	break;
	}
	return 0;
};

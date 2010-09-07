#include "stdapp.h"
#include "resource.h"
#include <commctrl.h>
#include <windowsx.h>

extern CMysInput2* g_pcMysInput;
char debugstr[MAX_PATH];

LRESULT CALLBACK StartDlgProc(HWND hDlg, UINT hInst, WPARAM wparam, LPARAM lparam)
{
//メモ：コールバック関数は何も処理しなかったとき0を返し、
//　　　何らかの処理をしたときは非0(TRUE)を返す約束だよ。
//	LPCTSTR ListComb_Str[BLOODTYPE_NUM] = {LIST_COMB_STR};
	WORD w=0;
	dword joynum=0;
	static int nNo;
	char ss[MAX_PATH+10]="PAD";
	switch(hInst)
	{
	case WM_INITDIALOG:
		OutputDebugString("ダイアログボックス初期化開始...\n");
		HWND hTab, hCtrl;
		TCITEM ti;
		
		hTab = GetDlgItem(hDlg, IDC_TAB);

		//ZeroMemory(&ti,sizeof(TCITEM));
		ti.mask = TCIF_TEXT;
		ti.pszText = "設定１";
		TabCtrl_InsertItem(hTab, 0, &ti);
		ti.pszText = "設定２";
		TabCtrl_InsertItem(hTab, 1, &ti);
		ti.pszText = "設定３";
		TabCtrl_InsertItem(hTab, 2, &ti);
		ti.pszText = "設定４";
		TabCtrl_InsertItem(hTab, 3, &ti);
		ti.pszText = "設定５";
		TabCtrl_InsertItem(hTab, 4, &ti);
		ti.pszText = "設定６";
		TabCtrl_InsertItem(hTab, 5, &ti);
		ti.pszText = "設定７";
		TabCtrl_InsertItem(hTab, 6, &ti);
		ti.pszText = "設定８";
		TabCtrl_InsertItem(hTab, 7, &ti);

//パッド情報表示
//※未完成コード※
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

//キーコンフィグ選択
		hCtrl = GetDlgItem(hDlg,IDC_COMBO1);
		ComboBox_InsertString(hCtrl,0,"設定１");
		ComboBox_InsertString(hCtrl,1,"設定２");
		ComboBox_InsertString(hCtrl,2,"設定３");
		ComboBox_InsertString(hCtrl,3,"設定４");
		ComboBox_InsertString(hCtrl,4,"設定５");
		ComboBox_InsertString(hCtrl,5,"設定６");
		ComboBox_InsertString(hCtrl,6,"設定７");
		ComboBox_InsertString(hCtrl,7,"設定８");
		ComboBox_SetCurSel(hCtrl, 0);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO2);
		ComboBox_InsertString(hCtrl,0,"設定１");
		ComboBox_InsertString(hCtrl,1,"設定２");
		ComboBox_InsertString(hCtrl,2,"設定３");
		ComboBox_InsertString(hCtrl,3,"設定４");
		ComboBox_InsertString(hCtrl,4,"設定５");
		ComboBox_InsertString(hCtrl,5,"設定６");
		ComboBox_InsertString(hCtrl,6,"設定７");
		ComboBox_InsertString(hCtrl,7,"設定８");
		ComboBox_SetCurSel(hCtrl, 1);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO3);
		ComboBox_InsertString(hCtrl,0,"設定１");
		ComboBox_InsertString(hCtrl,1,"設定２");
		ComboBox_InsertString(hCtrl,2,"設定３");
		ComboBox_InsertString(hCtrl,3,"設定４");
		ComboBox_InsertString(hCtrl,4,"設定５");
		ComboBox_InsertString(hCtrl,5,"設定６");
		ComboBox_InsertString(hCtrl,6,"設定７");
		ComboBox_InsertString(hCtrl,7,"設定８");
		ComboBox_SetCurSel(hCtrl, 2);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO4);
		ComboBox_InsertString(hCtrl,0,"設定１");
		ComboBox_InsertString(hCtrl,1,"設定２");
		ComboBox_InsertString(hCtrl,2,"設定３");
		ComboBox_InsertString(hCtrl,3,"設定４");
		ComboBox_InsertString(hCtrl,4,"設定５");
		ComboBox_InsertString(hCtrl,5,"設定６");
		ComboBox_InsertString(hCtrl,6,"設定７");
		ComboBox_InsertString(hCtrl,7,"設定８");
		ComboBox_SetCurSel(hCtrl, 3);

		hCtrl = GetDlgItem(hDlg,IDC_COMBO5);
		ComboBox_InsertString(hCtrl,0,"設定１");
		ComboBox_InsertString(hCtrl,1,"設定２");
		ComboBox_InsertString(hCtrl,2,"設定３");
		ComboBox_InsertString(hCtrl,3,"設定４");
		ComboBox_InsertString(hCtrl,4,"設定５");
		ComboBox_InsertString(hCtrl,5,"設定６");
		ComboBox_InsertString(hCtrl,6,"設定７");
		ComboBox_InsertString(hCtrl,7,"設定８");
		ComboBox_SetCurSel(hCtrl, 4);

		//LoadCfg();

		//SetTimerでWM_TIMER命令を受け取る
		SetTimer(hDlg,100,10,NULL);

		//ComboBox_SetCurSel(GetDlgItem(m_hDlg,IDC_COMBO1), m_cAtvset[0]);
		//ComboBox_SetCurSel(GetDlgItem(m_hDlg,IDC_COMBO2), m_cAtvset[1]);
		OutputDebugString("ダイアログボックス初期化完了\n");
	break;

	case WM_TIMER:
//		joynum = g_pcMysInput->GetJoyNum();
//		g_pcMysInput->GetKeyState(pKeyState);
//		g_pcMysInput->GetJoyState(joycfg[PL_1][JOYNUM], pDirState, pBtnState);

	break;

	case WM_COMMAND://なんらかの動作がダイアログボックスにされた
		switch(wparam)
		{//具体的なコマンド
		case IDOK://起動
			OutputDebugString("ＯＫ選択\n");
			EndDialog(hDlg,IDOK);//二番目の引数がDialogBox関数の戻り値だよ。
			return TRUE;
		break;
		case IDCANCEL://終了
			OutputDebugString("キャンセル選択\n");
			EndDialog(hDlg,IDCANCEL);//二番目の引数がDialogBox関数の戻り値だよ。
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
			OutputDebugString("タブ切り替え ");
			hTab = GetDlgItem(hDlg, IDC_TAB);
			nNo = TabCtrl_GetCurSel(hTab);
			OutputDebugString("タブnoは ");
			OutputDebugString(_itoa(nNo,debugstr,10));
			OutputDebugString("\n");
			//ChangeTab(nNo);
			{	//タブ切り替えごとにEDIT_UP～EDIT_B4までに
				//nNoの設定を出す
			}
			return TRUE;
		}
	break;
	}
	return 0;
};

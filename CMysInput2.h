//
// CMysInput2.h
// マルチプレイヤー対応入力デバイスクラス
//

#ifndef _INC_CMYSINPUT_H_
#define _INC_CMYSINPUT_H_


class CMysInput2 : public CMysInput
{
public:
	//キーボードの状態を取得 pKeyStateはDirectInputのキーコード(DIK_Aなど)を使用する
	void GetKeyState(byte pKeyState[/*256*/]);

	//ジョイパッドの状態を取得
	void GetJoyState(dword index, byte pDirState[/*4*/], byte pBtnState[/*32*/]);
	// index:取得するジョイパッドの番号
	// pDirState:方向キーの状態(ONなら1,OFFなら0)(サイズ４,left,right,up,downの順)
	// pBtnState:ボタンの状態(ONなら1,OFFなら0)(サイズ32)

	//ジョイパッドの数を取得する
	dword GetJoyNum() { return m_nJoynum; }

protected:

};

#endif //_INC_CMYSINPUT_H_

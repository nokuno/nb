
#ifndef _INC_CMAINGMAE_H_
#define _INC_CMAINGMAE_H_



// メインゲームを開始する通常関数
bool StartMainGame();
//入力の、押された瞬間を検出する
void GetDevEvent();
//ゲームメインクラス
class CMainGame
{

public:
	CMainGame(){};
	~CMainGame(){};

	bool Initialize();//初期化

	bool StepFrame();//メイン処理
	void Render();//全描画
	bool Main();//メイン関数（内部でStepFlame();とRender();を実行）
	void EndGame();

protected:
	CMysImageEx				m_cimgBlock1;
	CMysImageEx				m_cimgBlock2;

	CMysNumberImage			m_cFpsImage;
	void DrawFloor1();
	void DrawFloor2();
};

#endif //_INC_CMAINGMAE_H_

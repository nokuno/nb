
#ifndef _INC_CBOMB_H_
#define _INC_CBOMB_H_

class CBomb:public CObj
{
public:
	CBomb(CPlayer *pPlayer);
	virtual ~CBomb();
	virtual void StepFrame();
	virtual void Render();
	static void LoadImage();
	void Explode();		//爆発
	virtual bool ColPlayer(CPlayer *pl){pl->SetOffIceFlag();return false;};
	virtual bool OnPlayer(CPlayer *pl){return false;};
	virtual bool ColFire(int bombIndex){m_Timer=m_maxTimer-COMBO_TIMER;return false;};
	virtual bool SetableBomb(){return false;};
	virtual OBJTYPE IsTyoe(){return BOMB;};
	int GetCounter(){return m_Timer;};
protected:
	CPlayer *m_pPlayer;	//親のプレイヤ
	int m_Timer;		//タイマー：０からm_maxTimerまで
	int m_maxTimer;
	int m_fire;			//火力：０ならひとマス

	int m_anmCount;		//アニメカウンタ：フラグが変わるまで数える：０からMAX_ANMCOUNT-1まで
	int m_anmFrame;		//アニメフレーム：画像の左から何番目のフレームか：０からMAX_ANMFRAME-1の間
	int m_anmDirect;	//アニメの方向
	//ここから静的メンバ変数
	static CMysImageEx s_cimg;
	static CMysSoundEx sndSetBomb;
	static CMysSoundEx sndExplode;
	//ここからヘルパー関数
	void UpFire(int i);
	void DownFire(int i);
	void LeftFire(int i);
	void RightFire(int i);
	//ここから定数
	static const int MAX_TIMER;
	static const int MAX_ANMCOUNT;		//アニメカウンタの最大値
	static const int MAX_ANMFRAME;		//アニメフレームの数
	static const int COMBO_TIMER;		//連鎖時間
};
#endif

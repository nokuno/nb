//CFire.h
//炎クラス
extern int g_fChange;
enum FIREKIND
{
	FIRE_SINGLE,
	FIRE_CENTER,
	FIRE_LR,
	FIRE_UD,
	FIRE_U,
	FIRE_D,
	FIRE_L,
	FIRE_R,
};
class CFire:public CObj
{
public:
	CFire(FIREKIND ak,int x,int y,int bombIndex,DIRECTION direct);
	virtual ~CFire(){};
	virtual void StepFrame();
	virtual void Render();
	virtual bool OnPlayer(CPlayer *pl){if(!pl->GetMuteki())pl->Damage(1,m_direct,m_bombIndex);return true;};
	virtual bool ColPlayer(CPlayer *pl){return true;};
	virtual bool ColFire(int bombIndex){delete this;return true;};
	virtual bool SetableBomb(){return true;};
	virtual bool IsFire(){return true;};
	virtual OBJTYPE IsTyoe(){return FIRE;};
	FIREKIND GetFireKind(){return m_anmKind;};
	static void LoadImage();
private:
	int m_bombIndex;			//親の爆弾のインデックス
	FIREKIND m_anmKind;			//炎の種類(本物）
	FIREKIND m_renderKind;		//炎の種類（見た目）
	DIRECTION m_direct;			//プレイヤーを吹っ飛ばす方向
	int m_anmCount;
	int m_anmFrame;
	static CMysImageEx s_img;
	static const int MAX_ANM_COUNT;	//アニメが変化する時間
};
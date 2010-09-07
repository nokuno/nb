//CBlock.h
//ブロッククラス
class CBlock:public CObj
{
public:
	static int m_num;		//全体の初期数
	CBlock(int x,int y);
	virtual ~CBlock();
	virtual void Render();
	virtual void StepFrame(){};
	virtual bool ColPlayer(CPlayer *pl){pl->SetOffIceFlag();return false;};
	virtual bool OnPlayer(CPlayer *pl){pl->SetOffIceFlag();return false;};
	virtual bool ColFire(int bombIndex);
	virtual bool SetableBomb(){return true;};
	virtual OBJTYPE IsTyoe(){return BLOCK;};

	friend void InitBlock();		//初期化
	void Fire();					//炎上処理
	void SetItem(int ik){m_itemKind=ik;};
	int GetItem(){return m_itemKind;};
private:
	static CMysImageEx m_img;	//共通の画像
	void Initialize(int x,int y);
	int m_itemKind;				//中に含むアイテム
};
void InitBlock();
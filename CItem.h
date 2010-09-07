//CItem.h
//アイテムクラス
//
#define ITEM_SPEED 0
#define ITEM_FIRE 1
#define ITEM_BOMB 2
#define ITEM_MAX 3
extern int g_fChange;
class CItem:public CObj
{
public:
	CItem(int kind,int x,int y);
	virtual ~CItem(){g_fChange = 2;};
	virtual void StepFrame();
	virtual void Render();
	virtual bool ColPlayer(CPlayer* pl);
	virtual bool OnPlayer(CPlayer* pl);
	virtual bool ColFire(int bombIndex);
	virtual bool SetableBomb(){return true;};
	virtual OBJTYPE IsTyoe(){return ITEM;};
	static void LoadImage();
private:
	int m_kind;
	int m_anmCount;
	int m_anmFrame;
	int m_anmDirect;
	static CMysImageEx s_img;
	static CMysSoundEx s_snd;
	static const int MAX_ANMCOUNT;
	static const int MAX_ANMFRAME;
};
/*
	about m_kind
0:speed
1:fire
2:bomb
*/
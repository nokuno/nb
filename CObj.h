//CObj.h
//オブジェクトクラス
//ブロック・爆弾・アイテムおよびギミックはこれを継承する
//g_ppObj[GetIndex(x,y)]がNULLかどうかで
//x,yの位置にこれらオブジェクトが存在しないかどうかがわかる
//アイテムゲットや爆弾のキックなどは
//CObjにプレイヤーに触れたときの仮想関数として定義する。
enum OBJTYPE
{
	BLOCK,
	BOMB,
	FIRE,
	ERASE,
	ITEM,
};
class CObj
{
public:
	CObj(){};
	virtual ~CObj(){};
	virtual void StepFrame()=0;
	virtual void Render()=0;
	//ここから判定関係
	//返り値は「上に乗れるかどうか」
	//プレイヤーが乗ろうとしたとき
	virtual bool ColPlayer(CPlayer *pl)=0;
	//プレイヤーが真上に乗ったとき
	virtual bool OnPlayer(CPlayer *pl)=0;
	//火に当てられたとき(返し値は貫通できるかどうか）
	virtual bool ColFire(int bombIndex)=0;
	//真上に爆弾を置こうとしたとき
	virtual bool SetableBomb()=0;//trueは爆発します
	//炎(CFire)であるかどうか(節の生成用）
	virtual bool IsFire(){return false;};
	//オブジェクトの種類問い合わせ（ＡＩ用）
	virtual OBJTYPE IsTyoe()=0;
protected:
	int m_x;
	int m_y;
};
//g_ppObjを初期化
//ブロックなども初期化（同じプロファイルを読み込むため）
void InitObj();
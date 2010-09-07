class CMap
{
public:
	CMap(){};
	virtual ~CMap(){};
	virtual int StepFrame()=0;
	virtual void Render()=0;
	//プレイヤーが乗ろうとしたとき
	virtual bool ColPlayer(CPlayer *pl)=0;
	//プレイヤーが真上に乗ったとき
	virtual bool OnPlayer(CPlayer *pl)=0;
protected:
	int		m_x,m_y;
};
int InitMap();
class CFloor:public CMap
{
public:
	CFloor(int x,int y){m_x=x;m_y=y;};
	~CFloor(){};
	virtual int StepFrame(){return true;};
	virtual void Render();
	virtual bool ColPlayer(CPlayer *pl){return true;};
	virtual bool OnPlayer(CPlayer *pl){pl->SetOffIceFlag();return true;};
	static int InitFloor();
private:
	static CMysImageEx s_img;
};

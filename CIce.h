class CIce:public CMap
{
public:
	CIce(int x,int y){m_x=x;m_y=y;};
	~CIce(){};
	virtual int StepFrame(){return true;};
	virtual void Render();
	virtual bool ColPlayer(CPlayer *pl){return true;};
	virtual bool OnPlayer(CPlayer *pl){pl->SetOnIceFlag();return true;};
	static int Init();
private:
	static CMysImageEx s_img;
};

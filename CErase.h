//CErase.h
//‰ŠãƒNƒ‰ƒX
class CErase:public CObj
{
public:
	CErase(int m_itemKind,int x,int y,int bombIndex);
	~CErase(){};
	virtual void StepFrame();
	virtual void Render();
	virtual bool OnPlayer(CPlayer *pl){if(!pl->GetMuteki())pl->Damage(1,NEUTRAL,m_bombIndex);return true;}
	virtual bool ColPlayer(CPlayer *pl){return false;};
	virtual bool ColFire(int bombIndex){return false;};
	virtual bool SetableBomb(){return true;};
	static void LoadImage();
	static int GetEraseTime(){return MAX_ANM_COUNT * MAX_ANM_FRAME;};
	virtual OBJTYPE IsTyoe(){return ERASE;};
private:
	int m_bombIndex;
	int m_itemKind;
	int m_anmCount;
	int m_anmFrame;
	static CMysImageEx s_img;
	static const int MAX_ANM_COUNT;
	static const int MAX_ANM_FRAME;
};
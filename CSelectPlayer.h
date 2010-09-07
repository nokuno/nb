
class CSelectPlayer:public CTitle
{
public:
	CSelectPlayer(){Initialize();};
	virtual ~CSelectPlayer(){};
	virtual bool StepFrame();
	virtual void Render();
	virtual bool Return(){return true;};
	bool Initialize();
protected:
	void OnUp();
	void OnDown();
	void OnOk();
	void OnCancel();
	void OnLeft();
	void OnRight();
	int m_plstate[5];//0:human,1:computer,2:none
};

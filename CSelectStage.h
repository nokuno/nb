#define STAGE_NUM 4
class CSelectStage:public CTitle
{
public:
	CSelectStage(){Initialize();};
	virtual ~CSelectStage(){};
	virtual bool StepFrame();
	virtual void Render();
	virtual bool Return(){return true;};
	bool Initialize();
protected:
	CMysImageEx m_prev;
	void OnLeft();
	void OnRight();
	void OnOk();
	void OnCancel();
	void ConcludeStage();
};
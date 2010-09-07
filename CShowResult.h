
class CShowResult:public CTitle
{
public:
	CShowResult(){Initialize();};
	virtual ~CShowResult(){m_snd.DeleteAll();};
	virtual bool StepFrame();
	virtual void Render();
	virtual bool Return(){return true;};
	bool Initialize();
protected:
	int m_timer;
	void OnOk();
	CMysSoundEx m_snd;
};
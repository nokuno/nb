class CMap
{
public:
	CMap(){};
	virtual ~CMap(){};
	virtual int StepFrame()=0;
	virtual void Render()=0;
	//�v���C���[����낤�Ƃ����Ƃ�
	virtual bool ColPlayer(CPlayer *pl)=0;
	//�v���C���[���^��ɏ�����Ƃ�
	virtual bool OnPlayer(CPlayer *pl)=0;
protected:
	int		m_x,m_y;
};
int InitMap();
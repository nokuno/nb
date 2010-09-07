//CBlock.h
//�u���b�N�N���X
class CBlock:public CObj
{
public:
	static int m_num;		//�S�̂̏�����
	CBlock(int x,int y);
	virtual ~CBlock();
	virtual void Render();
	virtual void StepFrame(){};
	virtual bool ColPlayer(CPlayer *pl){pl->SetOffIceFlag();return false;};
	virtual bool OnPlayer(CPlayer *pl){pl->SetOffIceFlag();return false;};
	virtual bool ColFire(int bombIndex);
	virtual bool SetableBomb(){return true;};
	virtual OBJTYPE IsTyoe(){return BLOCK;};

	friend void InitBlock();		//������
	void Fire();					//���㏈��
	void SetItem(int ik){m_itemKind=ik;};
	int GetItem(){return m_itemKind;};
private:
	static CMysImageEx m_img;	//���ʂ̉摜
	void Initialize(int x,int y);
	int m_itemKind;				//���Ɋ܂ރA�C�e��
};
void InitBlock();
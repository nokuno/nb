
#ifndef _INC_CTITLE_H_
#define _INC_CTITLE_H_

//�^�C�g���֐�
int StartTitle(bool stage);

class CTitle;
class CSelectPlayer;
class CSelectStage;
class CShowResult;

extern CTitle*			pcActive;//�A�N�e�B�u�ȃN���X
extern CTitle*			pcTitle;
extern CSelectPlayer*	pcSelectPlayer;
extern CSelectStage*	pcSelectStage;
extern CShowResult*		pcShowResult;

class CTitle
{
public:
	CTitle(){Initialize();};
	virtual ~CTitle(){};
	virtual bool StepFrame();
	virtual void Render();
	virtual bool Return(){return false;};
	virtual bool Initialize();
protected:
	void OnUp();
	void OnDown();
	void OnOk();
	static CMysImageEx s_imgTitle;	//�^�C�g���摜
	static CMysImageEx s_imgBG;		//�w�i�摜
	static CMysImageEx s_imgStrs;	//�����摜
	int m_cursor;					//�J�[�\���ʒu
};

#endif //_INC_CMAINGMAE_H_

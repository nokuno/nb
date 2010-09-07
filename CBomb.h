
#ifndef _INC_CBOMB_H_
#define _INC_CBOMB_H_

class CBomb:public CObj
{
public:
	CBomb(CPlayer *pPlayer);
	virtual ~CBomb();
	virtual void StepFrame();
	virtual void Render();
	static void LoadImage();
	void Explode();		//����
	virtual bool ColPlayer(CPlayer *pl){pl->SetOffIceFlag();return false;};
	virtual bool OnPlayer(CPlayer *pl){return false;};
	virtual bool ColFire(int bombIndex){m_Timer=m_maxTimer-COMBO_TIMER;return false;};
	virtual bool SetableBomb(){return false;};
	virtual OBJTYPE IsTyoe(){return BOMB;};
	int GetCounter(){return m_Timer;};
protected:
	CPlayer *m_pPlayer;	//�e�̃v���C��
	int m_Timer;		//�^�C�}�[�F�O����m_maxTimer�܂�
	int m_maxTimer;
	int m_fire;			//�ΉF�O�Ȃ�Ђƃ}�X

	int m_anmCount;		//�A�j���J�E���^�F�t���O���ς��܂Ő�����F�O����MAX_ANMCOUNT-1�܂�
	int m_anmFrame;		//�A�j���t���[���F�摜�̍����牽�Ԗڂ̃t���[�����F�O����MAX_ANMFRAME-1�̊�
	int m_anmDirect;	//�A�j���̕���
	//��������ÓI�����o�ϐ�
	static CMysImageEx s_cimg;
	static CMysSoundEx sndSetBomb;
	static CMysSoundEx sndExplode;
	//��������w���p�[�֐�
	void UpFire(int i);
	void DownFire(int i);
	void LeftFire(int i);
	void RightFire(int i);
	//��������萔
	static const int MAX_TIMER;
	static const int MAX_ANMCOUNT;		//�A�j���J�E���^�̍ő�l
	static const int MAX_ANMFRAME;		//�A�j���t���[���̐�
	static const int COMBO_TIMER;		//�A������
};
#endif

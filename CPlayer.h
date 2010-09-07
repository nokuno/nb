
#ifndef _INC_CPLAYER_H_
#define _INC_CPLAYER_H_

#include <vector>
using namespace std;
//�����L�[���̓^�C�v
enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NEUTRAL,
};

//AI�p�s�����X�g
enum ACTION
{
	ACT_UP,
	ACT_DOWN,
	ACT_LEFT,
	ACT_RIGHT,
	ACT_NEUTRAL,
	ACT_BOMB,
	ACT_NULL,
};
//�s���Ƃ��̎��̕]���̃Z�b�g
typedef struct
{
	ACTION	act;	//�s��
	int		value;	//�]���_
}ACTDATA,*LPACTDATA;
//�v���C���[�N���X
class CPlayer
{
public:
	CPlayer(){};
	~CPlayer(){};
	void StepFrame();	//�t���[������
	void Render();		//�`�揈��
	void Damage(int damage,DIRECTION d,int bombIndex);		//�_���[�W

	//��������C�����C�������o�֐�
	void Initialize(int i){ReadMember(i);InitMember();};
	int GetFire(){return State.fire;};
	void DecBombNum(){m_bombNum--;};//�f�N�������g
	void IncBombNum(){m_bombNum++;};//�C���N�������g
	int GetBombNum(){return m_bombNum;};
	int GetX(){return m_x;};
	int GetY(){return m_y;};
	bool GetFlag(){return m_existFlag;};
	bool GetMuteki(){return m_mutekiCount!=0;};
	void IncSpeed(){if(State.speed<8)State.speed++;};
	void IncFire(){State.fire++;};
	void IncBomb(){State.bomb++;};
	void SetOnIceFlag(){m_iceFlag = 2;};
	void SetOffIceFlag(){if(m_iceFlag>0)m_iceFlag = 0;};
	//��������static�����o�֐�
	static void InitStatic();
private:
	bool m_existFlag;			//���݃t���O
	bool m_fCom;				//�R���s���[�^�ł��邩�ǂ����̃t���O
	CMysImageEx		m_cimg;
	CMysImageEx		m_cimgDamage;//�_���[�W�摜
	int m_number;				//�v���C���[�ԍ�
	DIRECTION devdir;			//�����L�[�A�㉟���D��łЂƂ����F��
	bool devBtn[4];				//�{�^������

	int m_x,m_y;				//���W
	int m_anmCount;				//�A�j���J�E���^�F�t���O���ς��܂Ő�����F�O����MAX_ANMCOUNT-1�܂�
	int m_anmFrame;				//�A�j���t���[���F�摜�̍����牽�Ԗڂ̃t���[�����F�O����MAX_ANMFRAME-1�̊�
	DIRECTION m_direct;			//����

	int m_bombNum;				//���ݔ��e���F�O����State.bomb�̊�
	int m_bombCount;			//���e�J�E���^�̏�����ԁF�ʏ�͂O
	int m_bombKind;				//���e�̎�ށF�ʏ�͂O

	bool m_damageFlag;			//������ђ��t���O�i������m_direct���Q�Ƃ̂��Ɓj
	int m_damageBombIndex;		//�_���[�W��^�������e�̎��ʔԍ�
	int m_damageCount;			//�_���[�W��^�������e�̔����������鎞�ԁi�����蔻��ɕK�v�j

	int m_iceFlag;				//�X�Ŋ����Ă�t���O
	int m_mutekiCount;			//���G�J�E���^�F�O����MAX_MUTEKICOUNT�܂�

	struct PLAYERSTATE			//�v���C���[�̃X�e�[�^�X���
	{
		int hp;
		int bomb;
		int fire;
		int speed;
	}State,maxState,initState;	//���݁A�ő�A�����̃X�e�[�^�X

	//��������w���p�[�֐�
	void Die();			//���S
	void InitMember();				//�����o�[������
	void ReadMember(int i);			//�����o�[�����l�ǂݍ���(�v���C���[�ԍ��j

	DIRECTION GetDevDir();			//�����L�[���͏���
	void OnB1();					//�m�[�}�����e�̃{�^��
	void OnUpMap();					//CMap�̏�ɂ���Ƃ�
	//��������ړ��p�֐�
	void Move();					//�ړ�
	void Blow();					//�������
	void Slip();					//�X�̏�ł��ׂ�
		bool OnLeft(int);				//�����L�[���́B�Ԃ�l�̓A�j���t���O
		bool OnRight(int);
		bool OnUp(int);
		bool OnDown(int);
		void AnmRgst(bool anmFlag);		//�A�j���o�^�B�����̓A�j���t���O
		bool ObjColPlayer(int index);	//�w��I�u�W�F�N�g���ʉ߉\��
	//��������C�����C��
	void RoundCoordinats(){m_x=round((float)m_x/32)*32;m_y=round((float)m_y/32)*32;};//���W���l�̌ܓ�
	//��������static�����o
	static CMysSoundEx	s_sndDeath;		//���̔ߖi��
	static CMysImageEx	s_imgKage;		//�e�摜
	//��������萔
	static const int MAX_ANMCOUNT;		//�A�j���J�E���^�̍ő�l
	static const int MAX_ANMFRAME;		//�A�j���t���[���̐�
	static const int MAX_MUTEKI_COUNT;	//���G����
	static const int MAX_CORRECT;		//�ő�ړ��␳��
	static const int MUTEKI_DAMAGE;
	static const int BLOW_SPEED;

	//��������`�h�֌W
	ACTION m_act;		//���݂̃A�N�V����
	CBomb* m_pFirstBomb;//�ŏ��̔��e
	static int s_cpuLevel;		//�b�o�t���x��
	void AIRutin();		//�S�Ă̂`�h�Ăяo����
	bool Serch(int,int,int,ACTION,ACTION,int,int,int,int);		//�T���̍ċA�֐�
	void Action();		//m_act�Ɋ�Â��s��
public:
	CBomb* GetBombPointer(){return m_pFirstBomb;};
	void InitBombPointer(){m_pFirstBomb = NULL;};
};

#endif

/*
	���W�ɂ���
�v���C���[�Ŏg�����W�͎O��ށB

�P�D�X�N���[�����Wsx,sy(�`�掞�̂݁j
�Q�D���΍��Wrx,ry�i�����Ǘ��Am_x,m_y�Ȃǁj
�R�D�u���b�N���Wbx,by�iINI�t�@�C���̋L�q�Ȃǁj

3->2
rx = bx * 32;
ry = by * 32;
2->1
sx = rx + MAP_X;
sy = ry + MAP_Y - 32;//�c�ɒ����Ԃ��ɂȂ�
2->3
bx = rx / 32;
by = ry / 32;

���e�Ƃ��A�C�e���Ƃ��A���W����������u���b�N���W�̂Ƃ��̓u���b�N���W���g�����Ƃɂ���B
���Ă��Q�����z��ɂ��邩�炻�̓Y�������u���b�N���W�ɂȂ�B
*/


#ifndef _INC_CMAINGMAE_H_
#define _INC_CMAINGMAE_H_



// ���C���Q�[�����J�n����ʏ�֐�
bool StartMainGame();
//���͂́A�����ꂽ�u�Ԃ����o����
void GetDevEvent();
//�Q�[�����C���N���X
class CMainGame
{

public:
	CMainGame(){};
	~CMainGame(){};

	bool Initialize();//������

	bool StepFrame();//���C������
	void Render();//�S�`��
	bool Main();//���C���֐��i������StepFlame();��Render();�����s�j
	void EndGame();

protected:
	CMysImageEx				m_cimgBlock1;
	CMysImageEx				m_cimgBlock2;

	CMysNumberImage			m_cFpsImage;
	void DrawFloor1();
	void DrawFloor2();
};

#endif //_INC_CMAINGMAE_H_

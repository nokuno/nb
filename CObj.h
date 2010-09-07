//CObj.h
//�I�u�W�F�N�g�N���X
//�u���b�N�E���e�E�A�C�e������уM�~�b�N�͂�����p������
//g_ppObj[GetIndex(x,y)]��NULL���ǂ�����
//x,y�̈ʒu�ɂ����I�u�W�F�N�g�����݂��Ȃ����ǂ������킩��
//�A�C�e���Q�b�g�┚�e�̃L�b�N�Ȃǂ�
//CObj�Ƀv���C���[�ɐG�ꂽ�Ƃ��̉��z�֐��Ƃ��Ē�`����B
enum OBJTYPE
{
	BLOCK,
	BOMB,
	FIRE,
	ERASE,
	ITEM,
};
class CObj
{
public:
	CObj(){};
	virtual ~CObj(){};
	virtual void StepFrame()=0;
	virtual void Render()=0;
	//�������画��֌W
	//�Ԃ�l�́u��ɏ��邩�ǂ����v
	//�v���C���[����낤�Ƃ����Ƃ�
	virtual bool ColPlayer(CPlayer *pl)=0;
	//�v���C���[���^��ɏ�����Ƃ�
	virtual bool OnPlayer(CPlayer *pl)=0;
	//�΂ɓ��Ă�ꂽ�Ƃ�(�Ԃ��l�͊ђʂł��邩�ǂ����j
	virtual bool ColFire(int bombIndex)=0;
	//�^��ɔ��e��u�����Ƃ����Ƃ�
	virtual bool SetableBomb()=0;//true�͔������܂�
	//��(CFire)�ł��邩�ǂ���(�߂̐����p�j
	virtual bool IsFire(){return false;};
	//�I�u�W�F�N�g�̎�ޖ₢���킹�i�`�h�p�j
	virtual OBJTYPE IsTyoe()=0;
protected:
	int m_x;
	int m_y;
};
//g_ppObj��������
//�u���b�N�Ȃǂ��������i�����v���t�@�C����ǂݍ��ނ��߁j
void InitObj();
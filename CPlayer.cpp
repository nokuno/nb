
#include "stdapp.h"
#include "nb.h"

CMysSoundEx CPlayer::s_sndDeath;
int CPlayer::s_cpuLevel;
CMysImageEx CPlayer::s_imgKage;
const int CPlayer::MAX_ANMCOUNT = 3;		//�A�j���J�E���^�̍ő�l
const int CPlayer::MAX_ANMFRAME = 7;		//�A�j���t���[���̐�
const int CPlayer::MAX_MUTEKI_COUNT = 600;	//���G����
const int CPlayer::MAX_CORRECT = 30;		//�ő�ړ��␳��
const int CPlayer::MUTEKI_DAMAGE = 60;	//���G����
const int CPlayer::BLOW_SPEED = 10;

//static�����o������
void CPlayer::InitStatic()
{
	char buffer[32];
	//��
	s_sndDeath.LoadSound(prfGetString(szIniFile,"snd","death",buffer));
	//CPU���x��
	s_cpuLevel = prfGetInt(szIniFile,"config","level");
	//�e�摜
	s_imgKage.LoadImage(prfGetString(szIniFile,"img","kage",buffer));
	s_imgKage.CreateMask();
}
//�����o�[�ϐ��ǂݍ��݁i�@��player[i]���@�j
void CPlayer::ReadMember(int i)
{
	char buffer[32];
	m_number = i;
	char strPlayer[32];//"playeri"
	sprintf( strPlayer, "player%d", i );

	//�摜�ǂݍ���
	m_cimg.LoadImage(prfGetString(szIniFile,"img",strPlayer,buffer));
	m_cimg.CreateMask();
	m_cimgDamage.LoadImage(prfGetString(szIniFile,"img","damage",buffer));
	m_cimgDamage.CreateMask();

	prfGetString(szStageFile,"player",strPlayer,buffer);	//���W�ǂݍ���
	char szPlayerX[32];//m_x�̕�����o�[�W����
	char *pBuffer=buffer;
	strdiv(szPlayerX,pBuffer,',');//�����_��szPlayerX=pBuffer-","

	//m_x,m_y�͑��΍��W
	m_x = atoi(szPlayerX) * 32;	//x���W
	m_y = atoi(pBuffer) * 32;//y���W
	//�b�o�t���A���݂��邩
	prfGetString(szIniFile,"config",strPlayer,buffer);
	if(strcmp(buffer,"none")==0)	m_existFlag = false;
	else							m_existFlag = true;
	if(strcmp(buffer,"computer")==0)	m_fCom = true;
	else								m_fCom = false;
	//��ԓǂݍ���
	State.hp = prfGetInt(szStageFile,"state","hp");
	State.bomb = prfGetInt(szStageFile,"state","bomb");
	State.fire = prfGetInt(szStageFile,"state","fire");
	State.speed = prfGetInt(szStageFile,"state","speed");
}
//�����o�[�ϐ��������i�ǂݍ��݈ȊO�j
void CPlayer::InitMember()
{
	m_direct = DOWN;
	m_anmCount = 0;
	m_anmFrame = 0;

	m_bombNum = 0;
	m_bombCount = 0;
	m_bombKind = 0;

	m_damageFlag = false;
	m_damageCount = 0;
	m_mutekiCount = 0;
	devdir = NEUTRAL;
	m_act = ACT_NEUTRAL;
	m_pFirstBomb = NULL;
}
//���C������
void CPlayer::StepFrame()
{
	if(m_existFlag)
		if(!m_fCom)
			GetDevDir();
		else
			AIRutin();
	if(m_damageFlag)
		Blow();
	else if(m_iceFlag)
		Slip();
	else
	{
		Move();
		if(devBtn[0])
			OnB1();
	}
	if(m_mutekiCount)
	{
		m_mutekiCount++;
		if(m_mutekiCount == MAX_MUTEKI_COUNT)
			m_mutekiCount = 0;
	}
	if(g_ppObj[Index(round((float)m_x/32),round((float)m_y/32))] != NULL)
		g_ppObj[Index(round((float)m_x/32),round((float)m_y/32))]->OnPlayer(this);

	if(m_damageCount)
		m_damageCount--;
}
//�ړ�����
void CPlayer::Move()
{
	bool anmFlag = false;//�A�j���t���O
	//�ړ�
	switch(devdir)
	{
	case LEFT:
		anmFlag = OnLeft(State.speed);
		break;
	case RIGHT:
		anmFlag = OnRight(State.speed);
		break;
	case UP:
		anmFlag = OnUp(State.speed);
		break;
	case DOWN:
		anmFlag = OnDown(State.speed);
		break;
	}
	if(anmFlag)
		if(g_ppMap[Index(round((float)m_x/32),round((float)m_y/32))] != NULL)
			g_ppMap[Index(round((float)m_x/32),round((float)m_y/32))]->OnPlayer(this);
	//�A�j���[�V�����o�^
	AnmRgst(anmFlag);
}
//�������
void CPlayer::Blow()
{
	int x=m_x;
	int y=m_y;
	switch(m_direct)
	{
	case LEFT:
		if(!OnLeft(BLOW_SPEED))
		{
			m_damageFlag = false;
			m_direct = RIGHT;
			m_x=x+10;m_y=y;
			RoundCoordinats();
		}
		break;
	case RIGHT:
		if(!OnRight(BLOW_SPEED))
		{
			m_damageFlag = false;
			m_direct = LEFT;
			m_x=x-10;m_y=y;
			RoundCoordinats();
		}
		break;
	case UP:
		if(!OnUp(BLOW_SPEED))
		{
			m_damageFlag = false;
			m_direct = DOWN;
			m_x=x;m_y=y+10;
			RoundCoordinats();
		}
		break;
	case DOWN:
		if(!OnDown(BLOW_SPEED))
		{
			m_damageFlag = false;
			m_direct = UP;
			m_x=x;m_y=y-10;
			RoundCoordinats();
		}
		break;
	}
}
//���ׂ�
void CPlayer::Slip()
{
	if(g_ppMap[Index(round((float)m_x/32),round((float)m_y/32))] != NULL)
		g_ppMap[Index(round((float)m_x/32),round((float)m_y/32))]->OnPlayer(this);
	switch(m_direct)
	{
	case LEFT:
		if(!OnLeft(State.speed))
			m_iceFlag = 0;
		break;
	case RIGHT:
		if(!OnRight(State.speed))
			m_iceFlag = 0;
		break;
	case UP:
		if(!OnUp(State.speed))
			m_iceFlag = 0;
		break;
	case DOWN:
		if(!OnDown(State.speed))
			m_iceFlag = 0;
		break;
	}
}
//�I�u�W�F�N�g�̓����蔻��
bool CPlayer::ObjColPlayer(int index)
{
	if(g_ppMap[index] != NULL)
		g_ppMap[index]->ColPlayer(this);
	if(g_ppObj[index] != NULL)
	{
		if(!g_ppObj[index]->ColPlayer(this))
		{
			m_x = round((float)m_x/32)*32;
			m_y = round((float)m_y/32)*32;
			return false;
		}
	}
	return true;
}
//�A�j���[�V�����o�^
void CPlayer::AnmRgst(bool anmFlag)
{
	if(anmFlag)
	{
		m_anmCount ++;
		if(m_anmCount == MAX_ANMCOUNT)//�A�j���t���[��(�R�}�j���ς��Ƃ���
		{
			m_anmCount = 0;
			m_anmFrame ++;
			if(m_anmFrame == MAX_ANMFRAME)
				m_anmFrame = 1;
		}
	}
	else
	{
		m_anmCount = 0;
		m_anmFrame = 0;
	}
}
//���e�ݒu
void CPlayer::OnB1()
{
	if(m_bombNum<State.bomb)//���݂̔��e�����ő吔��菬������
	{
		CBomb* pBomb = NULL;
		if(g_ppObj[Index(round((float)m_x/32),round((float)m_y/32))] == NULL)
			pBomb = new CBomb(this);
		else
		{
			if(g_ppObj[Index(round((float)m_x/32),round((float)m_y/32))]->SetableBomb())
			{
				delete g_ppObj[Index(round((float)m_x/32),round((float)m_y/32))];
				(pBomb = new CBomb(this))->Explode();
			}
		}
		if(pBomb)
			if(m_pFirstBomb)
				m_pFirstBomb = pBomb;
	}
}
//�`�揈��
void CPlayer::Render()
{
	if(m_mutekiCount%2==0)
	{
		//m_x,m_y���X�N���[�����W�ɕϊ�
		int sx = m_x + MAP_X;
		int sy = m_y + MAP_Y - 32;
		RECT rect;//�`��̈�
		if(!m_damageFlag)//�ʏ�
		{
			rect.left = m_anmFrame*32;
			rect.right = m_anmFrame*32+32;
			switch(m_direct)
			{
			case DOWN:
				rect.top = 0;
				rect.bottom = 64;
				break;
			case UP:
				rect.top = 64;
				rect.bottom = 128;
				break;
			case RIGHT:
				rect.top = 128;
				rect.bottom = 192;
				break;
			case LEFT:
				rect.top = 192;
				rect.bottom = 256;
				break;
			}
			m_cimg.BltRsv(m_y,sx,sy,&rect);//�]���\��
		}
		else//������ђ�
		{
			rect.top = 0;
			rect.bottom = 64;
			switch(m_direct)
			{
			case UP:
				rect.left = 0;
				rect.right = 32;
				break;
			case DOWN:
				rect.left = 32;
				rect.right = 64;
				break;
			case LEFT:
				rect.left = 64;
				rect.right = 96;
				break;
			case RIGHT:
				rect.left = 96;
				rect.right = 128;
				break;
			}
			m_cimgDamage.BltRsv(m_y,sx,sy,&rect);//�]���\��
		}
		RECT r={0,0,32,32};
		s_imgKage.BltRsv(-1,MAP_X+m_x,MAP_Y+m_y,&r);//�e
	}
}
//�㉟���D��ɂ��āA���݃A�N�e�B�u�ȕ����L�[��Ԃ�
DIRECTION CPlayer::GetDevDir()
{
	int i;
	//�{�^������
	for(i=0;i<4;i++)
		devBtn[i] = (devevent[m_number][i+4] == 1);
	//������������L�[
	for(i=0;i<4;i++)
	{
		if(devevent[m_number][i]==1)
		{//�����ꂽ�u��
			devdir = (DIRECTION) i;
			return devdir;
		}
	}
	for(i=0;i<4;i++)
	{
		if(devstate[m_number][i] && devdir == (DIRECTION) i)
		{//�O����A�N�e�B�u
			devdir = (DIRECTION) i;
			return devdir;
		}
	}
	for(i=0;i<4;i++)
	{
		if(devstate[m_number][i])
		{//�A�N�e�B�u�������̂������ꂽ�u�Ԃ���ȊO�ŉ�����Ă���Ƃ�
			devdir = (DIRECTION) i;
			return devdir;
		}
	}
	devdir = NEUTRAL;
	return devdir;
}
//���S�����B���񂾂�
void CPlayer::Die()
{
	m_existFlag = false;
	s_sndDeath.Play();
}
//�_���[�W
void CPlayer::Damage(int damage,DIRECTION d,int bombIndex)
{
	if(m_damageCount == 0 || bombIndex != m_damageBombIndex)
	{
		//m_mutekiCount = MAX_MUTEKI_COUNT-MUTEKI_DAMAGE;
		m_damageFlag = true;		//������ђ��t���O���n�m�ɂ���
		m_damageCount = CErase::GetEraseTime();
		State.hp -= damage;
		if(d==NEUTRAL)
			m_direct = static_cast<DIRECTION>(4*rand()/(RAND_MAX+1));
		else
			m_direct = d;
		m_damageBombIndex = bombIndex;
		if(State.hp<=0)
			Die();
	}
}
//���������ꂽ�Ƃ�
bool CPlayer::OnLeft(int speed)
{
	m_direct = LEFT;//��������
	//y���W�̒ʘH����̍������Ƃ�
	int dy = (m_y-32) % 64;//���̎��_�ł�0����64
	if(dy > 32) dy -= 64;//-32����32�ɒ���

	if(m_x <= 32)//�������ǂȂ�
		return false;
	
	//�u���b�N���W
	int bx = m_x/32;
	int by = (m_y-dy)/32;
	int index;
	if(abs(dy)<speed)
	{
		if(m_x%32==0 || (m_y%32==0&&m_x%32>32-speed))
		{
			if(m_x%32==0)
				index = Index(bx-1,by);
			else
				index = Index(bx,by);
			if(!ObjColPlayer(index))
				return false;
		}
		m_x -= speed - abs(dy);
		m_y -= dy;
		return true;
	}
	else if(abs(dy)<MAX_CORRECT)//�Ȃ���Ȃ����Ǖ␳���邩
	{
		index = Index(bx-1,by);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_y -= speed * dy/abs(dy);//dy�̕����������Ă���
		return true;
	}
	return false;
}
//�E�������ꂽ�Ƃ�
bool CPlayer::OnRight(int speed)
{
	m_direct = RIGHT;//�E������
	//�ʘH����̍������Ƃ�
	int dy = (m_y-32) % 64;//���̎��_�ł�0����64
	if(dy > 32) dy -= 64;//-32����32�ɒ���

	if(m_x >= (MAP_SIZE_X-2)*32)//�E�����ǂȂ�
		return false;
	//�u���b�N���W
	int bx = m_x/32;
	int by = (m_y-dy)/32;
	int index;
	if(abs(dy)<speed)//y�����Ɉړ��������u�Ȃ����ʒu�v�ɒB���邩
	{
		if(m_x%32==0 || (m_y%32==0&&m_x%32<speed))
		{
			index = Index(bx+1,by);
			if(!ObjColPlayer(index))
				return false;
		}
		m_x += speed - abs(dy);
		m_y -= dy;
		return true;
	}
	else if(abs(dy)<MAX_CORRECT)//�Ȃ���Ȃ����Ǖ␳���邩
	{
		index = Index(bx+1,by);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_y -= speed * dy/abs(dy);//dy�̕����������Ă���
		return true;
	}
	return false;
}
//�オ�����ꂽ�Ƃ�
bool CPlayer::OnUp(int speed)
{
	m_direct = UP;//�������
	//x���W�̒ʘH����̍������Ƃ�
	int dx = (m_x-32) % 64;//���̎��_�ł�0����64
	if(dx > 32) dx -= 64;//-32����32�ɒ���

	if(m_y <= 32)//�㑤���ǂȂ�
		return false;
	//�u���b�N���W
	int bx = (m_x-dx)/32;
	int by = m_y/32;
	int index;
	if(abs(dx)<speed)//x�����Ɉړ��������u�Ȃ����ʒu�v�ɒB���邩
	{
		if(m_y%32==0 || (m_x%32==0&&m_y%32>32-speed))
		{
			if(m_y%32==0)
				index = Index(bx,by-1);
			else
				index = Index(bx,by);
			if(!ObjColPlayer(index))
				return false;
		}
		m_y -= speed - abs(dx);
		m_x -= dx;
		return true;
	}
	else if(abs(dx)<MAX_CORRECT)//�Ȃ���Ȃ����Ǖ␳���邩
	{
		index = Index(bx,by-1);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_x -= speed * dx/abs(dx);//dx�̕����������Ă���
		return true;
	}
	return false;
}
//���������ꂽ�Ƃ�
bool CPlayer::OnDown(int speed)
{
	m_direct = DOWN;//��������
	//x���W�̒ʘH����̍������Ƃ�
	int dx = (m_x-32) % 64;//���̎��_�ł�0����64
	if(dx > 32) dx -= 64;//-32����32�ɒ���

	if(m_y >= (MAP_SIZE_Y-2)*32)//�������ǂȂ�
		return false;
	//�u���b�N���W
	int bx = (m_x-dx)/32;
	int by = m_y/32;
	int index;
	if(abs(dx)<speed)//x�����Ɉړ��������u�Ȃ����ʒu�v�ɒB���邩
	{
		if(m_y%32==0 || (m_x%32==0&&m_y%32<speed))
		{
			index = Index(bx,by+1);
			if(!ObjColPlayer(index))
				return false;
		}
		m_y += speed - abs(dx);
		m_x -= dx;
		return true;
	}
	else if(abs(dx)<MAX_CORRECT)//�Ȃ���Ȃ����Ǖ␳���邩
	{
		index = Index(bx,by+1);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_x -= speed * dx/abs(dx);//dx�̕����������Ă���
		return true;
	}
	return false;
}

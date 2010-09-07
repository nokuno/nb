#include "stdapp.h"
#include "nb.h"
const int CBomb::MAX_TIMER = 100;
const int CBomb::MAX_ANMCOUNT = 10;		//アニメカウンタの最大値
const int CBomb::MAX_ANMFRAME = 4;		//アニメフレームの数
const int CBomb::COMBO_TIMER = 3;
CMysImageEx CBomb::s_cimg;
CMysSoundEx	CBomb::sndSetBomb;
CMysSoundEx	CBomb::sndExplode;
void SetDanger(int,int,int);
void SetValueOrigin(int,int,int);

//static
void CBomb::LoadImage()
{
	char buffer[256];
	s_cimg.LoadImage(prfGetString(szIniFile,"img","bomb",buffer));
	s_cimg.CreateMask();
	sndSetBomb.LoadSound(prfGetString(szIniFile,"snd","setbomb",buffer));
	sndExplode.LoadSound(prfGetString(szIniFile,"snd","explode",buffer));
}
CBomb::CBomb(CPlayer *pPlayer)
{
	pPlayer->IncBombNum();

	m_x = round((float)pPlayer->GetX()/32);
	m_y = round((float)pPlayer->GetY()/32);
	m_fire = pPlayer->GetFire();
	m_pPlayer = pPlayer;
	m_Timer = 0;
	m_maxTimer = MAX_TIMER;
	m_anmCount = 0;
	m_anmFrame = 0;
	m_anmDirect = 1;

	g_ppObj[Index(m_x,m_y)] = this;
	sndSetBomb.Play();
	SetDanger(m_x,m_y,m_fire);
}
CBomb::~CBomb()
{
	SetValueOrigin(m_x,m_y,m_fire);
	m_pPlayer->DecBombNum();//親プレイヤのBombNumをデクリメント
	if(m_pPlayer->GetBombPointer() == this)
		m_pPlayer->InitBombPointer();
}
void CBomb::StepFrame()
{
	m_Timer++;
	if(m_Timer == 1)
		g_fChange = 1;//変化フラグをたてる
	//ここからアニメ関係
	m_anmCount ++;
	if(m_anmCount == MAX_ANMCOUNT)//アニメフレーム(コマ）が変わるところ
	{
		m_anmCount = 0;
		if(m_anmFrame == MAX_ANMFRAME-1)
			m_anmDirect = - m_anmDirect;
		else if(m_anmDirect==-1 && m_anmFrame == 0)
			m_anmDirect = - m_anmDirect;
		m_anmFrame += m_anmDirect;
	}
	if(m_Timer == MAX_TIMER)
		Explode();
}
void CBomb::Render()
{
	RECT rect;
	rect.left = m_anmFrame * 32;
	rect.top=0;
	rect.right = (m_anmFrame+1) * 32;
	rect.bottom=32;

	s_cimg.BltRsv(m_y*32,MAP_X+m_x*32,MAP_Y+m_y*32,&rect);
}
void CBomb::UpFire(int i)
{
	if(i==m_fire)
		new CFire(FIRE_U,m_x,m_y-i,Index(m_x,m_y),UP);
	else
		new CFire(FIRE_UD,m_x,m_y-i,Index(m_x,m_y),UP);
}
void CBomb::DownFire(int i)
{
	if(i==m_fire)
		new CFire(FIRE_D,m_x,m_y+i,Index(m_x,m_y),DOWN);
	else
		new CFire(FIRE_UD,m_x,m_y+i,Index(m_x,m_y),DOWN);
}
void CBomb::LeftFire(int i)
{
	if(i==m_fire)
		new CFire(FIRE_L,m_x-i,m_y,Index(m_x,m_y),LEFT);
	else
		new CFire(FIRE_LR,m_x-i,m_y,Index(m_x,m_y),LEFT);
}
void CBomb::RightFire(int i)
{
	if(i==m_fire)
		new CFire(FIRE_R,m_x+i,m_y,Index(m_x,m_y),RIGHT);
	else
		new CFire(FIRE_LR,m_x+i,m_y,Index(m_x,m_y),RIGHT);
}
void CBomb::Explode()
{
	sndExplode.Play();
	int i;
	//炎を残す
	if(m_fire == 0)
		new CFire(FIRE_SINGLE,m_x,m_y,Index(m_x,m_y),NEUTRAL);//single
	else
	{
		new CFire(FIRE_CENTER,m_x,m_y,Index(m_x,m_y),NEUTRAL);//center
		for(i=1;i<=m_fire;++i)//up
			if(m_y-i>0)
				if(g_ppObj[Index(m_x,m_y-i)] == NULL && (m_x%2==1||(m_y-i)%2==1))
					UpFire(i);
				else if(g_ppObj[Index(m_x,m_y-i)] != NULL && g_ppObj[Index(m_x,m_y-i)]->ColFire(Index(m_x,m_y)) && (m_x%2==1||(m_y-i)%2==1))
					UpFire(i);
				else
					break;
		for(i=1;i<=m_fire;++i)//down
			if(m_y+i<MAP_SIZE_Y-1)
				if(g_ppObj[Index(m_x,m_y+i)] == NULL && (m_x%2==1||(m_y+i)%2==1))
					DownFire(i);
				else if(g_ppObj[Index(m_x,m_y+i)] != NULL && g_ppObj[Index(m_x,m_y+i)]->ColFire(Index(m_x,m_y)) && (m_x%2==1||(m_y+i)%2==1))
					DownFire(i);
				else
					break;
		for(i=1;i<=m_fire;++i)//left
			if(m_x-i>0)
				if(g_ppObj[Index(m_x-i,m_y)] == NULL && ((m_x-i)%2==1||m_y%2==1))
					LeftFire(i);
				else if(g_ppObj[Index(m_x-i,m_y)] != NULL && g_ppObj[Index(m_x-i,m_y)]->ColFire(Index(m_x,m_y)) && ((m_x-1)%2==1||m_y%2==1))
					LeftFire(i);
				else
					break;
		for(i=1;i<=m_fire;++i)//right
			if(m_x+i<MAP_SIZE_X-1)
				if(g_ppObj[Index(m_x+i,m_y)] == NULL && ((m_x+i)%2==1||m_y%2==1))
					RightFire(i);
				else if(g_ppObj[Index(m_x+i,m_y)] != NULL && g_ppObj[Index(m_x+i,m_y)]->ColFire(Index(m_x,m_y)) && ((m_x+1)%2==1||m_y%2==1))
					RightFire(i);
				else
					break;
	}
	delete this;
}
//ＡＩ用評価を下げる
void SetDanger(int m_x,int m_y,int m_fire)
{
	int i;
	g_value[Index(m_x,m_y)] -= VALUE_BOMBDANGER;
	for(i=1;i<=m_fire;++i)//up
		if(m_y-i>0)
			if(m_x%2==1||(m_y-i)%2==1)
				g_value[Index(m_x,m_y-i)] -= VALUE_BOMBDANGER;
			else
				break;
	for(i=1;i<=m_fire;++i)//down
		if(m_y+i<MAP_SIZE_Y-1)
			if(m_x%2==1||(m_y+i)%2==1)
				g_value[Index(m_x,m_y+i)] -= VALUE_BOMBDANGER;
			else
				break;
	for(i=1;i<=m_fire;++i)//left
		if(m_x-i>0)
			if((m_x-i)%2==1||m_y%2==1)
				g_value[Index(m_x-i,m_y)] -= VALUE_BOMBDANGER;
			else
				break;
	for(i=1;i<=m_fire;++i)//right
		if(m_x+i<MAP_SIZE_X-1)
			if((m_x+i)%2==1||m_y%2==1)
				g_value[Index(m_x+i,m_y)] -= VALUE_BOMBDANGER;
			else
				break;
}
inline void DecreaseValue(int x,int y)
{
	if(g_value[Index(x,y)]<0)
		if(g_value[Index(x,y)]>-VALUE_BOMBDANGER)
			g_value[Index(x,y)]=0;
		else
			g_value[Index(x,y)] += VALUE_BOMBDANGER;
}
//下げた評価を元に戻す
void SetValueOrigin(int m_x,int m_y,int m_fire)
{
	int i;
	DecreaseValue(m_x,m_y);
	for(i=1;i<=m_fire;++i)//up
		if(m_y-i>0)
			if(m_x%2==1||(m_y-i)%2==1)
				DecreaseValue(m_x,m_y-i);
			else
				break;
	for(i=1;i<=m_fire;++i)//down
		if(m_y+i<MAP_SIZE_Y-1)
			if(m_x%2==1||(m_y+i)%2==1)
				DecreaseValue(m_x,m_y+i);
			else
				break;
	for(i=1;i<=m_fire;++i)//left
		if(m_x-i>0)
			if((m_x-i)%2==1||m_y%2==1)
				DecreaseValue(m_x-i,m_y);
			else
				break;
	for(i=1;i<=m_fire;++i)//right
		if(m_x+i<MAP_SIZE_X-1)
			if((m_x+i)%2==1||m_y%2==1)
				DecreaseValue(m_x+i,m_y);
			else
				break;
}
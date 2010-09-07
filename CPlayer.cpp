
#include "stdapp.h"
#include "nb.h"

CMysSoundEx CPlayer::s_sndDeath;
int CPlayer::s_cpuLevel;
CMysImageEx CPlayer::s_imgKage;
const int CPlayer::MAX_ANMCOUNT = 3;		//アニメカウンタの最大値
const int CPlayer::MAX_ANMFRAME = 7;		//アニメフレームの数
const int CPlayer::MAX_MUTEKI_COUNT = 600;	//無敵時間
const int CPlayer::MAX_CORRECT = 30;		//最大移動補正量
const int CPlayer::MUTEKI_DAMAGE = 60;	//無敵時間
const int CPlayer::BLOW_SPEED = 10;

//staticメンバ初期化
void CPlayer::InitStatic()
{
	char buffer[32];
	//音
	s_sndDeath.LoadSound(prfGetString(szIniFile,"snd","death",buffer));
	//CPUレベル
	s_cpuLevel = prfGetInt(szIniFile,"config","level");
	//影画像
	s_imgKage.LoadImage(prfGetString(szIniFile,"img","kage",buffer));
	s_imgKage.CreateMask();
}
//メンバー変数読み込み（ⅰはplayer[i]のⅰ）
void CPlayer::ReadMember(int i)
{
	char buffer[32];
	m_number = i;
	char strPlayer[32];//"playeri"
	sprintf( strPlayer, "player%d", i );

	//画像読み込み
	m_cimg.LoadImage(prfGetString(szIniFile,"img",strPlayer,buffer));
	m_cimg.CreateMask();
	m_cimgDamage.LoadImage(prfGetString(szIniFile,"img","damage",buffer));
	m_cimgDamage.CreateMask();

	prfGetString(szStageFile,"player",strPlayer,buffer);	//座標読み込み
	char szPlayerX[32];//m_xの文字列バージョン
	char *pBuffer=buffer;
	strdiv(szPlayerX,pBuffer,',');//ラムダのszPlayerX=pBuffer-","

	//m_x,m_yは相対座標
	m_x = atoi(szPlayerX) * 32;	//x座標
	m_y = atoi(pBuffer) * 32;//y座標
	//ＣＰＵか、存在するか
	prfGetString(szIniFile,"config",strPlayer,buffer);
	if(strcmp(buffer,"none")==0)	m_existFlag = false;
	else							m_existFlag = true;
	if(strcmp(buffer,"computer")==0)	m_fCom = true;
	else								m_fCom = false;
	//状態読み込み
	State.hp = prfGetInt(szStageFile,"state","hp");
	State.bomb = prfGetInt(szStageFile,"state","bomb");
	State.fire = prfGetInt(szStageFile,"state","fire");
	State.speed = prfGetInt(szStageFile,"state","speed");
}
//メンバー変数初期化（読み込み以外）
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
//メイン処理
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
//移動処理
void CPlayer::Move()
{
	bool anmFlag = false;//アニメフラグ
	//移動
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
	//アニメーション登録
	AnmRgst(anmFlag);
}
//吹き飛び
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
//すべり
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
//オブジェクトの当たり判定
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
//アニメーション登録
void CPlayer::AnmRgst(bool anmFlag)
{
	if(anmFlag)
	{
		m_anmCount ++;
		if(m_anmCount == MAX_ANMCOUNT)//アニメフレーム(コマ）が変わるところ
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
//爆弾設置
void CPlayer::OnB1()
{
	if(m_bombNum<State.bomb)//現在の爆弾数が最大数より小さいか
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
//描画処理
void CPlayer::Render()
{
	if(m_mutekiCount%2==0)
	{
		//m_x,m_yをスクリーン座標に変換
		int sx = m_x + MAP_X;
		int sy = m_y + MAP_Y - 32;
		RECT rect;//描画領域
		if(!m_damageFlag)//通常
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
			m_cimg.BltRsv(m_y,sx,sy,&rect);//転送予約
		}
		else//吹っ飛び中
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
			m_cimgDamage.BltRsv(m_y,sx,sy,&rect);//転送予約
		}
		RECT r={0,0,32,32};
		s_imgKage.BltRsv(-1,MAP_X+m_x,MAP_Y+m_y,&r);//影
	}
}
//後押し優先にして、現在アクティブな方向キーを返す
DIRECTION CPlayer::GetDevDir()
{
	int i;
	//ボタン入力
	for(i=0;i<4;i++)
		devBtn[i] = (devevent[m_number][i+4] == 1);
	//ここから方向キー
	for(i=0;i<4;i++)
	{
		if(devevent[m_number][i]==1)
		{//押された瞬間
			devdir = (DIRECTION) i;
			return devdir;
		}
	}
	for(i=0;i<4;i++)
	{
		if(devstate[m_number][i] && devdir == (DIRECTION) i)
		{//前からアクティブ
			devdir = (DIRECTION) i;
			return devdir;
		}
	}
	for(i=0;i<4;i++)
	{
		if(devstate[m_number][i])
		{//アクティブだったのが離された瞬間それ以外で押されているとき
			devdir = (DIRECTION) i;
			return devdir;
		}
	}
	devdir = NEUTRAL;
	return devdir;
}
//死亡処理。こんだけ
void CPlayer::Die()
{
	m_existFlag = false;
	s_sndDeath.Play();
}
//ダメージ
void CPlayer::Damage(int damage,DIRECTION d,int bombIndex)
{
	if(m_damageCount == 0 || bombIndex != m_damageBombIndex)
	{
		//m_mutekiCount = MAX_MUTEKI_COUNT-MUTEKI_DAMAGE;
		m_damageFlag = true;		//吹っ飛び中フラグをＯＮにする
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
//左が押されたとき
bool CPlayer::OnLeft(int speed)
{
	m_direct = LEFT;//左を向く
	//y座標の通路からの差分をとる
	int dy = (m_y-32) % 64;//この時点では0から64
	if(dy > 32) dy -= 64;//-32から32に直す

	if(m_x <= 32)//左側が壁なら
		return false;
	
	//ブロック座標
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
	else if(abs(dy)<MAX_CORRECT)//曲がらないけど補正するか
	{
		index = Index(bx-1,by);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_y -= speed * dy/abs(dy);//dyの符号をかけている
		return true;
	}
	return false;
}
//右が押されたとき
bool CPlayer::OnRight(int speed)
{
	m_direct = RIGHT;//右を向く
	//通路からの差分をとる
	int dy = (m_y-32) % 64;//この時点では0から64
	if(dy > 32) dy -= 64;//-32から32に直す

	if(m_x >= (MAP_SIZE_X-2)*32)//右側が壁なら
		return false;
	//ブロック座標
	int bx = m_x/32;
	int by = (m_y-dy)/32;
	int index;
	if(abs(dy)<speed)//y方向に移動した時「曲がれる位置」に達するか
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
	else if(abs(dy)<MAX_CORRECT)//曲がらないけど補正するか
	{
		index = Index(bx+1,by);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_y -= speed * dy/abs(dy);//dyの符号をかけている
		return true;
	}
	return false;
}
//上が押されたとき
bool CPlayer::OnUp(int speed)
{
	m_direct = UP;//上を向く
	//x座標の通路からの差分をとる
	int dx = (m_x-32) % 64;//この時点では0から64
	if(dx > 32) dx -= 64;//-32から32に直す

	if(m_y <= 32)//上側が壁なら
		return false;
	//ブロック座標
	int bx = (m_x-dx)/32;
	int by = m_y/32;
	int index;
	if(abs(dx)<speed)//x方向に移動した時「曲がれる位置」に達するか
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
	else if(abs(dx)<MAX_CORRECT)//曲がらないけど補正するか
	{
		index = Index(bx,by-1);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_x -= speed * dx/abs(dx);//dxの符号をかけている
		return true;
	}
	return false;
}
//下が押されたとき
bool CPlayer::OnDown(int speed)
{
	m_direct = DOWN;//下を向く
	//x座標の通路からの差分をとる
	int dx = (m_x-32) % 64;//この時点では0から64
	if(dx > 32) dx -= 64;//-32から32に直す

	if(m_y >= (MAP_SIZE_Y-2)*32)//下側が壁なら
		return false;
	//ブロック座標
	int bx = (m_x-dx)/32;
	int by = m_y/32;
	int index;
	if(abs(dx)<speed)//x方向に移動した時「曲がれる位置」に達するか
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
	else if(abs(dx)<MAX_CORRECT)//曲がらないけど補正するか
	{
		index = Index(bx,by+1);
		if(g_ppObj[index] != NULL)
			if(!g_ppObj[index]->ColPlayer(this))
				return false;
		m_x -= speed * dx/abs(dx);//dxの符号をかけている
		return true;
	}
	return false;
}

#include "stdapp.h"
#include "nb.h"
void CPlayer::AIRutin()
{
	int dx = m_x%32;		if(dx > 16) dx -= 16;
	int dy = m_y%32;	if(dy > 16) dy -= 16;
	if(!m_damageFlag)
		if((g_time%30==m_number*30/5)||
			(abs(dx)<State.speed && abs(dy)<State.speed))
			Serch(0,round((float)m_x/32),round((float)m_y/32),ACT_NEUTRAL,ACT_NEUTRAL,0,0,0,0);
	Action();
}
//[‚³A‚˜A‚™A’¼‘O‚Ìs“®A•]‰¿’†s“®A”š’e‚ð’u‚¢‚½‚Æ‚«‚Ì•]‰¿
bool CPlayer::Serch(int depth,int x,int y,ACTION act,ACTION nowAct,int bombValue,int bx,int by,int proValue)
{
	static int max_value;					//Œ»Ý‚ÌÅ‚•]‰¿“_
	CVirtualBomb* pvb;						//‰¼‘z”š’e
	//‰Šú‰»
	if(depth==0)
		max_value = -1000;
	if(depth == 1)
		nowAct = act;//Å‰‚Ì•]‰¿‚ÍŽÀÛ‚É•]‰¿‚·‚és“®
	//“Ç‚Þ–¢—ˆ‚Ì[‚³‚ð’´‚¦‚Ä‚¢‚é
	if(depth > s_cpuLevel)
		return true;
	//”š’e‚ð’u‚­
	if(act == ACT_BOMB)
	{
		pvb = new CVirtualBomb(x,y,State.fire);
		if((x%2==1) && (y%2==1))
			bombValue+=VALUE_CROSSBOMB;//\Žš˜H
		else
			bombValue+=VALUE_BOMBSET;
	}
	int vl;
	if(depth > 0)
		if(act!=ACT_BOMB)
		{
			vl =GetValue(x,y,depth,bombValue);
			if(vl-proValue>=max_value)
			{
				max_value = vl-proValue;
				m_act = nowAct;
			}
			proValue = vl/VALUE_PROCESS;
		}
	switch(act)
	{
	case ACT_BOMB:
		if(GetCan(x+1,y,bx,by))
			if(!Serch(depth+1,x+1,y,ACT_RIGHT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x-1,y,bx,by))
			if(!Serch(depth+1,x-1,y,ACT_LEFT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y-1,bx,by))
			if(!Serch(depth+1,x,y-1,ACT_UP,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y+1,bx,by))
			if(!Serch(depth+1,x,y+1,ACT_DOWN,nowAct,bombValue,bx,by,proValue)) return false;
		break;
	case ACT_NEUTRAL:
		if(GetCan(x+1,y,bx,by))
			if(!Serch(depth+1,x+1,y,ACT_RIGHT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x-1,y,bx,by))
			if(!Serch(depth+1,x-1,y,ACT_LEFT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y-1,bx,by))
			if(!Serch(depth+1,x,y-1,ACT_UP,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y+1,bx,by))
			if(!Serch(depth+1,x,y+1,ACT_DOWN,nowAct,bombValue,bx,by,proValue)) return false;
		break;
	case ACT_UP:
		if(GetCan(x+1,y,bx,by))
			if(!Serch(depth+1,x+1,y,ACT_RIGHT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x-1,y,bx,by))
			if(!Serch(depth+1,x-1,y,ACT_LEFT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y-1,bx,by))
			if(!Serch(depth+1,x,y-1,ACT_UP,nowAct,bombValue,bx,by,proValue)) return false;
		break;
	case ACT_DOWN:
		if(GetCan(x+1,y,bx,by))
			if(!Serch(depth+1,x+1,y,ACT_RIGHT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x-1,y,bx,by))
			if(!Serch(depth+1,x-1,y,ACT_LEFT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y+1,bx,by))
			if(!Serch(depth+1,x,y+1,ACT_DOWN,nowAct,bombValue,bx,by,proValue)) return false;
		break;
	case ACT_LEFT:
		if(GetCan(x-1,y,bx,by))
			if(!Serch(depth+1,x-1,y,ACT_LEFT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y-1,bx,by))
			if(!Serch(depth+1,x,y-1,ACT_UP,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y+1,bx,by))
			if(!Serch(depth+1,x,y+1,ACT_DOWN,nowAct,bombValue,bx,by,proValue)) return false;
		break;
	case ACT_RIGHT:
		if(GetCan(x+1,y,bx,by))
			if(!Serch(depth+1,x+1,y,ACT_RIGHT,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y-1,bx,by))
			if(!Serch(depth+1,x,y-1,ACT_UP,nowAct,bombValue,bx,by,proValue)) return false;
		if(GetCan(x,y+1,bx,by))
			if(!Serch(depth+1,x,y+1,ACT_DOWN,nowAct,bombValue,bx,by,proValue)) return false;
		break;
	}
	if(act!=ACT_BOMB)
	{
		if(!Serch(depth+1,x,y,ACT_NEUTRAL,nowAct,bombValue,bx,by,proValue)) return false;
		if(m_bombNum<State.bomb && (!m_bombNum || m_pFirstBomb))
			if((bx!=x||by!=y)&&!g_ppObj[Index(x,y)])
				if(!Serch(depth+1,x,y,ACT_BOMB,nowAct,bombValue,x,y,proValue)) return false;
	}
	if(act==ACT_BOMB)
		delete pvb;
	return true;
}
void CPlayer::Action()
{
	if(devBtn[0])
		devBtn[0]=false;
	switch(m_act)
	{
	case ACT_UP:
		devdir = UP;
		break;
	case ACT_DOWN:
		devdir = DOWN;
		break;
	case ACT_LEFT:
		devdir = LEFT;
		break;
	case ACT_RIGHT:
		devdir = RIGHT;
		break;
	case ACT_NEUTRAL:
		devdir = NEUTRAL;
		break;
	case ACT_BOMB:
		devdir = NEUTRAL;
		devBtn[0] = true;
		m_act = ACT_NULL;
		break;
	}
	if(m_pFirstBomb)
		if(m_pFirstBomb->GetCounter() < 20)
			m_pFirstBomb = NULL;
}
int GetValue(int x,int y,int time,int bombValue)
{
	int ret = bombValue - time * VALUE_TIME;
	if(g_ppObj[Index(x,y)])
	{
		if(g_ppObj[Index(x,y)]->IsTyoe() == ITEM)
			ret +=VALUE_ITEM;
		else if(g_ppObj[Index(x,y)]->IsTyoe() == BOMB)
			ret -= VALUE_BOMBDANGER;
		else if(g_ppObj[Index(x,y)]->IsTyoe() == FIRE)
			ret -= VALUE_BOMBDANGER;
	}
	ret += g_value[Index(x,y)];
	return ret;
}

bool GetCan(int x,int y,int bx,int by)
{
	if(x==bx&&y==by)
		return false;
	if(!(
		(x%2==1||y%2==1)&&
		(x!=0&&y!=0&&x!=MAP_SIZE_X-1&&y!=MAP_SIZE_Y-1)
		))//•Ç
		return false;
	if(g_ppObj[Index(x,y)])
		if(g_ppObj[Index(x,y)]->IsTyoe() != ITEM)
			return false;
	return true;
}

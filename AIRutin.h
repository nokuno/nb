int GetValue(int x,int y,int time,int bobmValue);
bool GetCan(int x,int y,int bx,int by);
//‚`‚h—p‰¼‘z”š’e
class CVirtualBomb
{
public:
	CVirtualBomb(int x,int y,int fire){m_x=x;m_y=y;m_fire=fire;SetDanger();};
	~CVirtualBomb(){SetValueOrigin();};
private:
	int m_x,m_y,m_fire;
	//‚`‚h—p•]‰¿‚ð‰º‚°‚é
	void SetDanger()
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
	//‰º‚°‚½•]‰¿‚ðŒ³‚É–ß‚·
	void SetValueOrigin()
	{
		int i;
		g_value[Index(m_x,m_y)] += VALUE_BOMBDANGER;
		for(i=1;i<=m_fire;++i)//up
			if(m_y-i>0)
				if(m_x%2==1||(m_y-i)%2==1)
					g_value[Index(m_x,m_y-i)] += VALUE_BOMBDANGER;
				else
					break;
		for(i=1;i<=m_fire;++i)//down
			if(m_y+i<MAP_SIZE_Y-1)
				if(m_x%2==1||(m_y+i)%2==1)
					g_value[Index(m_x,m_y+i)] += VALUE_BOMBDANGER;
				else
					break;
		for(i=1;i<=m_fire;++i)//left
			if(m_x-i>0)
				if((m_x-i)%2==1||m_y%2==1)
					g_value[Index(m_x-i,m_y)] += VALUE_BOMBDANGER;
				else
					break;
		for(i=1;i<=m_fire;++i)//right
			if(m_x+i<MAP_SIZE_X-1)
				if((m_x+i)%2==1||m_y%2==1)
					g_value[Index(m_x+i,m_y)] += VALUE_BOMBDANGER;
				else
					break;
	}
};
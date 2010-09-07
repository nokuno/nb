
#ifndef _INC_CTITLE_H_
#define _INC_CTITLE_H_

//タイトル関数
int StartTitle(bool stage);

class CTitle;
class CSelectPlayer;
class CSelectStage;
class CShowResult;

extern CTitle*			pcActive;//アクティブなクラス
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
	static CMysImageEx s_imgTitle;	//タイトル画像
	static CMysImageEx s_imgBG;		//背景画像
	static CMysImageEx s_imgStrs;	//文字画像
	int m_cursor;					//カーソル位置
};

#endif //_INC_CMAINGMAE_H_

#include "stdapp.h"
#include "nb.h"

int CBlock::m_num;//staticメンバの実体
CMysImageEx CBlock::m_img;
CBlock::CBlock(int x,int y)
{
	m_itemKind = FLAG_UNINI;
	m_x=x;
	m_y=y;
	g_ppObj[Index(m_x,m_y)]=this;
}
CBlock::~CBlock(){}
void CBlock::Initialize(int x,int y)
{
	m_x = x;
	m_y = y;
	g_ppObj[Index(x,y)] = this;
}
//ブロック（B0)でないマスの数
int GetNoBlock(char **prfArray)
{
	int result = 0;
	for(int i=0;i<MAP_SIZE;i++)
		if(strcmp(prfArray[i],"B0"))//B0と一致しないとき
			result++;

	return result;
}
//ランダムに配列初期化
void GetExistFlag(int *existFlag,char** prfArray,int noBlock)
{
	int i,j;
	for(i=0;i<MAP_SIZE;i++)
	{
		if(strcmp(prfArray[i],"B0"))
			existFlag[i] = FLAG_FALSE;
		else
			existFlag[i] = FLAG_UNINI;//未初期化
	}
	for(i=0;i<CBlock::m_num;i++)		//trueの回数ぶんループ
	{
		int n = (MAP_SIZE-noBlock - i) * rand()/(RAND_MAX+1);	//i 回目のn は（MAP_SQUARE-i)の空間を持つ
		for(j = 0;j <= n;j++)
		{
			if(existFlag[j]!=FLAG_UNINI)
			{
				n++;
			}
		}
		existFlag[n] = FLAG_TRUE;
	}
}
//アイテム数げと
inline int GetItemNum(int *a)
{
	a[0] = prfGetInt(szStageFile,"item","speed");
	a[1] = prfGetInt(szStageFile,"item","fire");
	a[2] = prfGetInt(szStageFile,"item","bomb");
	return a[0]+a[1]+a[2];
}
//アイテム実体生成
void GetItemFlag(int *existFlag)
{//existFlagはあくまでブロックの存在フラグなんで気をつけろ。
	int item[ITEM_MAX];
	int itemNum=GetItemNum(item);
	int i,j;
	int *itemFlag = new int[MAP_SIZE];
	//初期化
	for(i=0;i<MAP_SIZE;i++){
		itemFlag[i]=FLAG_UNINI;//未初期化
		if(g_ppObj[i]!=NULL)
			((CBlock*)g_ppObj[i])->SetItem(FLAG_UNINI);
	}
	//フラグげと
	for(i=0;i<itemNum;i++)
	{
		int n = (CBlock::m_num - i) * rand()/(RAND_MAX+1);
		for(j = 0;j <= n;j++)
		{
			if(g_ppObj[j]==NULL || itemFlag[j] != FLAG_UNINI)
			{//ブロックが存在しないorアイテムがすでに存在する
				n++;
			}
		}
		//実体生成
		if(i<item[0]){
			itemFlag[n] = ITEM_SPEED;
			((CBlock*)g_ppObj[n])->SetItem(ITEM_SPEED);
		}
		else if(i<item[1]+item[0]){
			itemFlag[n] = ITEM_FIRE;
			((CBlock*)g_ppObj[n])->SetItem(ITEM_FIRE);
		}
		else if(i<item[2]+item[1]+item[0]){
			itemFlag[n] = ITEM_BOMB;
			((CBlock*)g_ppObj[n])->SetItem(ITEM_BOMB);
		}
	}
	delete itemFlag;
}
//初期化。なぜかstaticメンバではなくフレンド関数
void InitBlock()
{
	int i,j;
	int *existFlag;
	CBlock::m_num = prfGetInt(szStageFile,"item","block");	//初期ブロック数
	//プロファイル確保
	char **prfArray = new char*[MAP_SIZE];	//プロファイル配列（一次元）
	for(i=0;i<MAP_SIZE;i++)
		prfArray[i] = new char[3];//ひとつのプロファイルは二文字+ヌル文字
	GetObjProfile(prfArray,"obj");
	//プロファイル確保終了

	int noBlock = GetNoBlock(prfArray);
	existFlag = new int[MAP_SIZE];
	GetExistFlag(existFlag,prfArray,noBlock);
	//プロファイル削除
	for(i=0;i<MAP_SIZE;i++)
		delete[] prfArray[i];
	delete[] prfArray;
	//ここから登録作業
	for(j=0;j<MAP_SIZE_Y;j++)
		for(i=0;i<MAP_SIZE_X;i++)
			if(existFlag[Index(i,j)]==FLAG_TRUE)
				new CBlock(i,j);

	//ここからアイテム関係
	GetItemFlag(existFlag);
	delete[] existFlag;
	char buffer[32];
	CBlock::m_img.LoadImage(prfGetString(szStageFile,"img","block2",buffer));	//画像読み込み
	CBlock::m_img.CreateMask();
}
void CBlock::Render()
{
	RECT rect;
	rect.left=0;
	rect.top=0;
	rect.right=32;
	rect.bottom=64;

	m_img.BltRsv(m_y*32,MAP_X+m_x*32,MAP_Y+(m_y-1)*32,&rect);
}
bool CBlock::ColFire(int bombIndex)
{
	g_ppObj[Index(m_x,m_y)] = new CErase(m_itemKind,m_x,m_y,bombIndex);
	delete this;
	return false;
}
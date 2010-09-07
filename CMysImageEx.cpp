#include "CMysImageEx.h"

inline word MAKERGB16(byte r, byte g, byte b)
{
	return (word)(r&0xF8)<<11 | (word)(g&0xFC)<<5 | (word)(b&0xF8);
}
//staticメンバ変数の実体生成
list<BLTRSV> CMysImageEx::m_BltList;

//BLYRSV構造体の比較演算子（ソートのために必要）
bool BLTRSV::operator<(BLTRSV bltrsv){
	return (sprLevel < bltrsv.sprLevel);
}
bool BLTRSV::operator>(BLTRSV bltrsv){
	return (sprLevel > bltrsv.sprLevel);
}
CMysImageEx::CMysImageEx(){};
CMysImageEx::~CMysImageEx(){};

//転送予約を取る
void CMysImageEx::BltRsv(int sprLevel,int iDestX,int iDestY,LPRECT prcSrc,int mask_index)
{
	BLTRSV br;
	br.pImgSrc = this;				//thisポインタを保存しておく
	br.sprLevel = sprLevel;
	br.iDestX = iDestX;
	br.iDestY = iDestY;
	br.rcSrc = *prcSrc;
	br.mask_index = mask_index;

	m_BltList.push_front(br);
}
//予約を全て転送
//staticメンバ関数
void CMysImageEx::BltAll()
{
	m_BltList.sort();				//ソート
	list<BLTRSV>::iterator itr;		//list<BLTRSV>のイタレータ
	for(itr = m_BltList.begin();itr != m_BltList.end();++itr)
	{
		(*itr->pImgSrc).mysBlt(
			NULL,
			itr->iDestX,
			itr->iDestY,
			&(itr->rcSrc),
			itr->mask_index
		);
	}
	m_BltList.clear();				//リストを全てクリア
}
void CMysImageEx::ClearBuffer()
{
	CMysImage img;
	img.CreateImage(640,480);
	img.FillImage(MAKERGB16(0,0,0));
	img.mysBlt(NULL,0,0);
}
bool CMysImageEx::LoadImage(LPCTSTR lpszFileName)
{
	DeleteImage();
	return CMysImage::LoadImage(lpszFileName);
}
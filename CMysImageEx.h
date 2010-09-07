/*-----------------------------------------------------
	
	CMysImageEx.h

    拡張イメージクラス

------------------------------------------------------*/

#pragma once

#include <list>
using namespace std;

#include "Mys\\CMysImage.h"
/*
//転送モード
enum BLTMODE
{
	NORMAL,
	ALPHA,
	ADDBLEND,
	SUBBLEND,
};
*/
//転送予約情報構造体（転送先はバックバッファで固定です）
struct BLTRSV
{
	CMysImage *pImgSrc;//転送元。thisポインタを保存しておく
	int sprLevel;//スプライトレベル（大きいほど手前）
	int iDestX;
	int iDestY;
	RECT rcSrc;
	int mask_index;
//	BLTMODE blt_mode;//未実装
//	BYTE bAlphaKey;
	
	bool operator<(BLTRSV bltrsv);//比較演算子（ソートに必要）
	bool operator>(BLTRSV bltrsv);//
};

typedef BLTRSV* LPBLTRSV;

//拡張イメージクラス
//staticばっかでごめんなさい
class CMysImageEx:public CMysImage
{
public:
	CMysImageEx();
	~CMysImageEx();

	//転送予約を取る(CMysImage参照。アルファブレンド・加算合成・減算合成には未対応）
	void BltRsv(int sprLevel,int iDestX,int iDestY,LPRECT prcSrc,int mask_index=0);
	//予約を全て転送・Rsv予約を削除
	static void BltAll();
	//バックバッファを黒でクリア
	static void ClearBuffer();
/*
以下未実装	
	//転送登録をする
	void BltRgst(LPBLTRSV lpBltRsv);
	//Rgst予約も含めてクリアー
	static void ClearList();
*/
	virtual bool LoadImage(LPCTSTR lpszFileName);
	static void ClearList(){m_BltList.clear();};
private:
	static list<BLTRSV> m_BltList;	//予約リスト
};

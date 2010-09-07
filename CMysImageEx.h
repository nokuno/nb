/*-----------------------------------------------------
	
	CMysImageEx.h

    �g���C���[�W�N���X

------------------------------------------------------*/

#pragma once

#include <list>
using namespace std;

#include "Mys\\CMysImage.h"
/*
//�]�����[�h
enum BLTMODE
{
	NORMAL,
	ALPHA,
	ADDBLEND,
	SUBBLEND,
};
*/
//�]���\����\���́i�]����̓o�b�N�o�b�t�@�ŌŒ�ł��j
struct BLTRSV
{
	CMysImage *pImgSrc;//�]�����Bthis�|�C���^��ۑ����Ă���
	int sprLevel;//�X�v���C�g���x���i�傫���قǎ�O�j
	int iDestX;
	int iDestY;
	RECT rcSrc;
	int mask_index;
//	BLTMODE blt_mode;//������
//	BYTE bAlphaKey;
	
	bool operator<(BLTRSV bltrsv);//��r���Z�q�i�\�[�g�ɕK�v�j
	bool operator>(BLTRSV bltrsv);//
};

typedef BLTRSV* LPBLTRSV;

//�g���C���[�W�N���X
//static�΂����ł��߂�Ȃ���
class CMysImageEx:public CMysImage
{
public:
	CMysImageEx();
	~CMysImageEx();

	//�]���\������(CMysImage�Q�ƁB�A���t�@�u�����h�E���Z�����E���Z�����ɂ͖��Ή��j
	void BltRsv(int sprLevel,int iDestX,int iDestY,LPRECT prcSrc,int mask_index=0);
	//�\���S�ē]���ERsv�\����폜
	static void BltAll();
	//�o�b�N�o�b�t�@�����ŃN���A
	static void ClearBuffer();
/*
�ȉ�������	
	//�]���o�^������
	void BltRgst(LPBLTRSV lpBltRsv);
	//Rgst�\����܂߂ăN���A�[
	static void ClearList();
*/
	virtual bool LoadImage(LPCTSTR lpszFileName);
	static void ClearList(){m_BltList.clear();};
private:
	static list<BLTRSV> m_BltList;	//�\�񃊃X�g
};

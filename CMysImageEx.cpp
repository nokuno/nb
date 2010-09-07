#include "CMysImageEx.h"

inline word MAKERGB16(byte r, byte g, byte b)
{
	return (word)(r&0xF8)<<11 | (word)(g&0xFC)<<5 | (word)(b&0xF8);
}
//static�����o�ϐ��̎��̐���
list<BLTRSV> CMysImageEx::m_BltList;

//BLYRSV�\���̂̔�r���Z�q�i�\�[�g�̂��߂ɕK�v�j
bool BLTRSV::operator<(BLTRSV bltrsv){
	return (sprLevel < bltrsv.sprLevel);
}
bool BLTRSV::operator>(BLTRSV bltrsv){
	return (sprLevel > bltrsv.sprLevel);
}
CMysImageEx::CMysImageEx(){};
CMysImageEx::~CMysImageEx(){};

//�]���\������
void CMysImageEx::BltRsv(int sprLevel,int iDestX,int iDestY,LPRECT prcSrc,int mask_index)
{
	BLTRSV br;
	br.pImgSrc = this;				//this�|�C���^��ۑ����Ă���
	br.sprLevel = sprLevel;
	br.iDestX = iDestX;
	br.iDestY = iDestY;
	br.rcSrc = *prcSrc;
	br.mask_index = mask_index;

	m_BltList.push_front(br);
}
//�\���S�ē]��
//static�����o�֐�
void CMysImageEx::BltAll()
{
	m_BltList.sort();				//�\�[�g
	list<BLTRSV>::iterator itr;		//list<BLTRSV>�̃C�^���[�^
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
	m_BltList.clear();				//���X�g��S�ăN���A
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
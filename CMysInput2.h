//
// CMysInput2.h
// �}���`�v���C���[�Ή����̓f�o�C�X�N���X
//

#ifndef _INC_CMYSINPUT_H_
#define _INC_CMYSINPUT_H_


class CMysInput2 : public CMysInput
{
public:
	//�L�[�{�[�h�̏�Ԃ��擾 pKeyState��DirectInput�̃L�[�R�[�h(DIK_A�Ȃ�)���g�p����
	void GetKeyState(byte pKeyState[/*256*/]);

	//�W���C�p�b�h�̏�Ԃ��擾
	void GetJoyState(dword index, byte pDirState[/*4*/], byte pBtnState[/*32*/]);
	// index:�擾����W���C�p�b�h�̔ԍ�
	// pDirState:�����L�[�̏��(ON�Ȃ�1,OFF�Ȃ�0)(�T�C�Y�S,left,right,up,down�̏�)
	// pBtnState:�{�^���̏��(ON�Ȃ�1,OFF�Ȃ�0)(�T�C�Y32)

	//�W���C�p�b�h�̐����擾����
	dword GetJoyNum() { return m_nJoynum; }

protected:

};

#endif //_INC_CMYSINPUT_H_

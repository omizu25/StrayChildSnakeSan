//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �N���X
//==================================================
class CTexture
{
	/* ����`�� */
public:
	enum ELabel
	{
		LABEL_NONE = -1,	// �g�p���Ȃ�
		LABEL_Effect = 0,	// �G�t�F�N�g
		LABEL_Cursor,		// �J�[�\��
		LABEL_Number,		// ����
		LABEL_MAX
	};

	/* �������o�֐��� */
public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public:
	void LoadAll();				// �S�Ă̓ǂݍ���
	void Load(ELabel label);	// �w��̓ǂݍ���
	void ReleaseAll();			// �S�Ă̔j��
	void Release(ELabel label);	// �w��̔j��
	LPDIRECT3DTEXTURE9 Get(ELabel label);	// ���̎擾

	/* �������o�ϐ��� */
private:
	LPDIRECT3DTEXTURE9 m_pTexture[LABEL_MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
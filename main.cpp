//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g���Ƃ��͕K�v

//�}�N����`
#define GAME_TITLE	"�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH	1280				//�Q�[����ʂ̕�(�E�B�h�X)
#define GAME_HEIGHT	720					//�Q�[����ʂ̍���(�n�C�g)
#define GAME_COLOR  32					//�Q�[���̐F��

#define GAME_ICON_ID	333				//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[


// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���


	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�l�p�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;			//���̒��S
	int Y = GAME_HEIGHT / 2;		//�c�̒��S

	//�l�p�̑傫�������߂�
	int width = 32;		//��
	int height = 32;	//����


	//�~�̔��a�����߂�
	int radius = 100;

	//���x
	int speed = 10;
	int Xspeed = speed;
	int Yspeed = speed;

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1)
	{
		//��������L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)	//0�̂Ƃ��ɁA�����L�[�������ꂽ
		{
			break;	//�������[�v�𔲂���
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
		{
			break;	//�������[�v�𔲂���
		}

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�l�p��`��
		//DrawBox(
		//	X, Y, X+ width, Y+height,
		//	GetColor(255, 0, 0),		//�F���擾
		//	TRUE						//�h��Ԃ��H
		//);

		//�~��`��
		DrawCircle(X, Y, radius, GetColor(0, 255, 0), FALSE, 5);

		X += Xspeed;				//�l�p�̈ʒu���E�ɂ��炷
		Y += Yspeed;				//�l�p�̈ʒu�����ɂ��炷

		//����@�@�l�p���΂߉E���ɓ��������I
					//height�̍���Y+�BY++�ŉ��ɂ��炷�B

		//����A�@�l�p����ʂ̒[�ɗ�����A�ړ���������𔽓]�����Ă݂悤
		if (X - radius < 0|| X + radius > GAME_WIDTH)	//����ʂ���o���Ƃ�
		{
			//�ړ����x�̕����𔽓]������
			//+1�Ȃ�A-1	-1�Ȃ�+1
			Xspeed = -Xspeed;			//�ړ���������𔽓]

			//�ǂɓ��������瑁���Ȃ�
			if (Xspeed > 0) { Xspeed += 2; }
			else if (Xspeed < 0) { Xspeed -= 2; }

			//

		}

		if (Y - radius< 0 || Y + radius > GAME_HEIGHT)	//�c��ʂ���o���Ƃ�
		{
			//�ړ����x�̕����𔽓]������
			//+1�Ȃ�A-1	-1�Ȃ�+1
			Yspeed = -Yspeed;			//�ړ���������𔽓]

			if (Yspeed > 0) { Yspeed += 2; }
			else if (Yspeed < 0) { Yspeed -= 2; }
		}

		ScreenFlip();					//�_�u���o�b�t�@�����O������ʂ�`��
	}







	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}
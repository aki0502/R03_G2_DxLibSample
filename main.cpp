//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�\���̂̒�`

//�L�����N�^�̍\����
struct CHARACTOR
{
	int handle = -1;		//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];			//�摜�̏ꏊ�i�p�X�j
	int x;					//x�̈ʒu
	int y;					//y�̈ʒu
	int width;				//��
	int height;				//����
	int speed = 1;			//�ړ����x

	RECT Coll;				//�����蔻��̗̈�(�l�p)
	BOOL IsDraw;			//�摜���`��ł���H
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;	//�ߋ��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���V�[��

//�v���C���[
CHARACTOR player;

//�S�[��
 CHARACTOR Goal;


//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

int fadeTimeMill = 2000;		//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;					//�����l
int fadeOutCnt = fadeOutCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;		//�����l
int fadeInCnt = fadeInCntInit;			//�t�F�[�h�C���̃J�E���^
int fadeInCntMax = 0;					//�t�F�[�h�C���̃J�E���^MAX


//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʁi�����j
VOID EndDraw(VOID);		//�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCENE scene);		//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);		//�����蔻��̗̈�
VOID CollUpdate(CHARACTOR* chara);				//�����蔻��
BOOL Oncollision(RECT coll1, RECT coll2);		//��`�Ƌ�`�̓����蔻��



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
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���	//�f�B�X�v���C�̕`�摬�x��TRUE�ő҂AFALSE�Ŗ{���̑��x�ɂȂ�B
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���


	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.handle, &player.width, &player.height);

	

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);



	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����


	//�v���C���[�̉摜��ǂݍ���
	strcpyDx(player.path, ".\\image\\Player.jpg");
	player.handle = LoadGraph(player.path);		//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ��������́A�G���[(-1)������
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			player.path,					//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",			//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);
			DxLib_End();					//�����I��
		return -1;							//�G���[�I��
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.handle, &player.width, &player.height);

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
	}
	/*
	//�����蔻����X�V����
	CollUpdatePlayer(&player);		//�v���C���[�̓����蔻��̍X�V
	*/

	//�S�[���̓����蔻����X�V����
	CollUpdate(&Goal);

	//�v���C���[��������
	player.x = GAME_WIDTH / 2 - player.width / 2;		//������
	player.y = GAME_HEIGHT / 2 - player.width / 2;		//������
	player.speed = 500;
	player.IsDraw = TRUE;	//�`��ł���I


	//�S�[���摜��ǂݍ���
	strcpyDx(Goal.path, ".\\image\\blue�n���n���S�[��.jpg");
	Goal.handle = LoadGraph(Goal.path);		//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ��������́A�G���[(-1)������
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			Goal.path,					//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",			//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);
		DxLib_End();					//�����I��
		return -1;							//�G���[�I��
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//�S�[����������
	Goal.x = GAME_WIDTH - Goal.width;
	Goal.y = 0;
	Goal.speed = 500;		//�X�s�[�h
	Goal.IsDraw = TRUE;		//�`��ł���I

	//�������[�v
	while (1)
	{
	

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����

		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//END���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;			//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;		//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();					//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I��������̏���
	DeleteGraph(player.handle);			//�摜���������ォ��폜
	DeleteGraph(Goal.handle);			//�摜���������ォ��폜




	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}

/// <summary>
/// �؂�ւ����
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;		//�V�[����؂�ւ�
	IsFadeIn = FALSE;		//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;		//�t�F�[�h�A�E�g����
}

/// <summary>
/// �^�C�g�����
/// </summary>
/// <param name=""></param>
VOID Title(VOID)
{
	TitleProc();		//����
	TitleDraw();		//�`��

	return ;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏��������s���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}
	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
/// <param name=""></param>
VOID Play(VOID)
{
	PlayProc();		//����
	PlayDraw();		//�`��

	return;
}

/// <summary>
/// �S�[���̕`��
/// </summary>
/// <param name=""></param>
VOID GoalProc(VOID)
{
	

	//�S�[����`��
	if (Goal.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

			//�f�o�b�N�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p�`��`��
			DrawBox(Goal.Coll.left, Goal.Coll.top, Goal.Coll.right, Goal.Coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	
	/*if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏��������s��

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}*/

	//�v���C���[���S�[���ɓ�����������
	if (Oncollision(player.Coll, Goal.Coll) == TRUE)
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	//�����蔻����X�V����
	CollUpdatePlayer(&player);
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//�v���C���[��`��
	if (player.IsDraw == TRUE)
	{

		//�摜��`��
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//�f�o�b�N�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p�`��`��
			DrawBox(player.Coll.left, player.Coll.top, player.Coll.right, player.Coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
		DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
		return;
	}
}

/// <summary>
/// �G���h���
/// </summary>
/// <param name=""></param>
VOID End(VOID)
{
	EndProc();		//����
	EndDraw();		//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏��������s��

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
/// <param name=""></param>
VOID Change(VOID)
{
	ChangeProc();		//����
	ChangeDraw();		//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
/// <param name=""></param>
VOID ChangeProc(VOID)
{
	
		//�t�F�[�h�C��
		if (IsFadeIn == TRUE)
		{
			if (fadeInCnt > fadeInCntMax)
			{
				fadeInCnt--;	//�J�E���g�����炷
			}
			else
			{
				//�t�F�[�h�������I�����

				fadeInCnt = fadeInCntInit;
				IsFadeIn = FALSE;
			}
		}

		//�t�F�[�h�A�E�g
		if (IsFadeOut == TRUE)
		{
			if (fadeOutCnt < fadeOutCntMax)
			{
				fadeOutCnt++;	//�J�E���g�����炷
			}
			else
			{
				//�t�F�[�h�������I�����

				fadeOutCnt = fadeOutCntInit;
				IsFadeOut = FALSE;
			}
		}
		
		//�؂�ւ������I��
		if (IsFadeIn == FALSE && IsFadeOut == FALSE)
		{
			//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
			GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
			OldGameScene = GameScene;
		}
	
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->Coll.left = chara->x;					//�����蔻��̒���
	chara->Coll.top = chara->y;						//�����蔻��̒���
	chara->Coll.right = chara->x + chara->width;	//�����蔻��̒���
	chara->Coll.bottom = chara->y + chara->height;	//�����蔻��̒���
	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();			//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawString(0, 0, " ", GetColor(0, 0, 0));
	return;
}
/// <summary>
/// �����蔻��̗̈�X�V(�v���C���[)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->Coll.left = chara->x;					//�����蔻��̒���
	chara->Coll.top = chara->y;						//�����蔻��̒���
	chara->Coll.right = chara->x + chara->width;	//�����蔻��̒���
	chara->Coll.bottom = chara->y + chara->height;	//�����蔻��̒���
	return;
}

//�����ōl���Ă�

//RECT coll1
typedef struct tagRECT {
	LONG left=100;		//����[�̂����W
	LONG top=30;		//�����[��y���W
	LONG right=350;		//�E��[�̂����W
	LONG bottom=130;	//�E���[��y���W

}RECT, * PRECT;

//RECT coll2
typedef struct tagRECT {
	LONG left = 290;		//����[�̂����W
	LONG top = 105;			//����[��y���W
	LONG right = 440;		//�E���[�̂����W
	LONG bottom = 290;		//�E���[��y���W

}RECT, * PRECT;

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="coll1">��`1</param>
/// <param name="coll2">��`2</param>
/// <returns></returns>
BOOL Oncollision(RECT coll1, RECT coll2)
{
	if (
		coll1.left < coll2.right &&		//���ӂ�X���W�@���@�E�ӂ̂w���W����
		coll1.right > coll2.left &&		//�E�ӂ̂w���W���@���ӂ̂w���W����
		coll1.top < coll2.bottom &&		//��ӂ̂x���W�����ӂ̂x���W����
		coll1.bottom > coll2.top		//���ӂ̂x���W����ӂ̂x���W
		)
	{
		//�����Ă�Ƃ�
		return TRUE;
	}
	else
	{
		//�����ĂȂ��Ƃ�
		return FALSE;
	}
}
//ヘッダファイルの読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//構造体の定義

//キャラクタの構造体
struct CHARACTOR
{
	int handle = -1;		//画像のハンドル（管理番号）
	char path[255];			//画像の場所（パス）
	int x;					//xの位置
	int y;					//yの位置
	int width;				//幅
	int height;				//高さ
	int speed = 1;			//移動速度

	RECT Coll;				//当たり判定の領域(四角)
	BOOL IsDraw;			//画像が描画できる？
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームシーン
GAME_SCENE OldGameScene;	//過去のゲームシーン
GAME_SCENE NextGameScene;	//次のゲームシーン

//プレイヤー
CHARACTOR player;

//ゴール
 CHARACTOR Goal;


//画面の切り替え
BOOL IsFadeOut = FALSE;	//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

int fadeTimeMill = 2000;		//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;

//フェードアウト
int fadeOutCntInit = 0;					//初期値
int fadeOutCnt = fadeOutCntInit;		//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;		//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;		//初期値
int fadeInCnt = fadeInCntInit;			//フェードインのカウンタ
int fadeInCntMax = 0;					//フェードインのカウンタMAX


//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面（処理）
VOID TitleDraw(VOID);	//タイトル画面（描画）

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面（処理）
VOID PlayDraw(VOID);	//プレイ画面（描画）

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面（処理）
VOID EndDraw(VOID);		//エンド画面（描画）

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面（処理）
VOID ChangeDraw(VOID);	//切り替え画面（描画）

VOID ChangeScene(GAME_SCENE scene);		//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);		//当たり判定の領域
VOID CollUpdate(CHARACTOR* chara);				//当たり判定
BOOL Oncollision(RECT coll1, RECT coll2);		//矩形と矩形の当たり判定



// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする	//ディスプレイの描画速度をTRUEで待つ、FALSEで本来の速度になる。
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする


	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);

	

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);



	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化


	//プレイヤーの画像を読み込み
	strcpyDx(player.path, ".\\image\\Player.jpg");
	player.handle = LoadGraph(player.path);		//画像の読み込み

	//画像が読み込めなあった時は、エラー(-1)が入る
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メインのウィンドウハンドル
			player.path,					//メッセージ本文
			"画像読み込みエラー！",			//メッセージタイトル
			MB_OK							//ボタン
		);
			DxLib_End();					//強制終了
		return -1;							//エラー終了
	}

	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);

	//プレイヤーの操作
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
	//当たり判定を更新する
	CollUpdatePlayer(&player);		//プレイヤーの当たり判定の更新
	*/

	//ゴールの当たり判定を更新する
	CollUpdate(&Goal);

	//プレイヤーを初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;		//中央寄せ
	player.y = GAME_HEIGHT / 2 - player.width / 2;		//中央寄せ
	player.speed = 500;
	player.IsDraw = TRUE;	//描画できる！


	//ゴール画像を読み込み
	strcpyDx(Goal.path, ".\\image\\blueハムハムゴール.jpg");
	Goal.handle = LoadGraph(Goal.path);		//画像の読み込み

	//画像が読み込めなあった時は、エラー(-1)が入る
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メインのウィンドウハンドル
			Goal.path,					//メッセージ本文
			"画像読み込みエラー！",			//メッセージタイトル
			MB_OK							//ボタン
		);
		DxLib_End();					//強制終了
		return -1;							//エラー終了
	}

	//画像の幅と高さを取得
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//ゴールを初期化
	Goal.x = GAME_WIDTH - Goal.width;
	Goal.y = 0;
	Goal.speed = 500;		//スピード
	Goal.IsDraw = TRUE;		//描画できる！

	//無限ループ
	while (1)
	{
	

		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける

		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//END画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;			//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;		//画面切り替えシーンに変える
			}
		}

		

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();					//ダブルバッファリングした画面を描画
	}

	//終わった時の処理
	DeleteGraph(player.handle);			//画像をメモリ上から削除
	DeleteGraph(Goal.handle);			//画像をメモリ上から削除




	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}

/// <summary>
/// 切り替え画面
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;		//シーンを切り替え
	IsFadeIn = FALSE;		//フェードインしない
	IsFadeOut = TRUE;		//フェードアウトする
}

/// <summary>
/// タイトル画面
/// </summary>
/// <param name=""></param>
VOID Title(VOID)
{
	TitleProc();		//処理
	TitleDraw();		//描画

	return ;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化を行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}
	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
/// <param name=""></param>
VOID Play(VOID)
{
	PlayProc();		//処理
	PlayDraw();		//描画

	return;
}

/// <summary>
/// ゴールの描画
/// </summary>
/// <param name=""></param>
VOID GoalProc(VOID)
{
	

	//ゴールを描画
	if (Goal.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

			//デバックのときは、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角形を描画
			DrawBox(Goal.Coll.left, Goal.Coll.top, Goal.Coll.right, Goal.Coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}
}

/// <summary>
/// プレイ画面の処理
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	
	/*if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化を行う

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}*/

	//プレイヤーがゴールに当たった時は
	if (Oncollision(player.Coll, Goal.Coll) == TRUE)
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	//当たり判定を更新する
	CollUpdatePlayer(&player);
	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//プレイヤーを描画
	if (player.IsDraw == TRUE)
	{

		//画像を描画
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//デバックのときは、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角形を描画
			DrawBox(player.Coll.left, player.Coll.top, player.Coll.right, player.Coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
		DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
		return;
	}
}

/// <summary>
/// エンド画面
/// </summary>
/// <param name=""></param>
VOID End(VOID)
{
	EndProc();		//処理
	EndDraw();		//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
/// <param name=""></param>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化を行う

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
/// <param name=""></param>
VOID Change(VOID)
{
	ChangeProc();		//処理
	ChangeDraw();		//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
/// <param name=""></param>
VOID ChangeProc(VOID)
{
	
		//フェードイン
		if (IsFadeIn == TRUE)
		{
			if (fadeInCnt > fadeInCntMax)
			{
				fadeInCnt--;	//カウントを減らす
			}
			else
			{
				//フェード処理が終わった

				fadeInCnt = fadeInCntInit;
				IsFadeIn = FALSE;
			}
		}

		//フェードアウト
		if (IsFadeOut == TRUE)
		{
			if (fadeOutCnt < fadeOutCntMax)
			{
				fadeOutCnt++;	//カウントを減らす
			}
			else
			{
				//フェード処理が終わった

				fadeOutCnt = fadeOutCntInit;
				IsFadeOut = FALSE;
			}
		}
		
		//切り替え処理終了
		if (IsFadeIn == FALSE && IsFadeOut == FALSE)
		{
			//フェードインしていない、フェードアウトもしていないとき
			GameScene = NextGameScene;	//次のシーンに切り替え
			OldGameScene = GameScene;
		}
	
	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->Coll.left = chara->x;					//当たり判定の調節
	chara->Coll.top = chara->y;						//当たり判定の調節
	chara->Coll.right = chara->x + chara->width;	//当たり判定の調節
	chara->Coll.bottom = chara->y + chara->height;	//当たり判定の調節
	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();			//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawString(0, 0, " ", GetColor(0, 0, 0));
	return;
}
/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->Coll.left = chara->x;					//当たり判定の調節
	chara->Coll.top = chara->y;						//当たり判定の調節
	chara->Coll.right = chara->x + chara->width;	//当たり判定の調節
	chara->Coll.bottom = chara->y + chara->height;	//当たり判定の調節
	return;
}

//自分で考えてる

//RECT coll1
typedef struct tagRECT {
	LONG left=100;		//左上端のｘ座標
	LONG top=30;		//左下端のy座標
	LONG right=350;		//右上端のｘ座標
	LONG bottom=130;	//右下端のy座標

}RECT, * PRECT;

//RECT coll2
typedef struct tagRECT {
	LONG left = 290;		//左上端のｘ座標
	LONG top = 105;			//左上端のy座標
	LONG right = 440;		//右下端のｘ座標
	LONG bottom = 290;		//右下端のy座標

}RECT, * PRECT;

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="coll1">矩形1</param>
/// <param name="coll2">矩形2</param>
/// <returns></returns>
BOOL Oncollision(RECT coll1, RECT coll2)
{
	if (
		coll1.left < coll2.right &&		//左辺のX座標　＜　右辺のＸ座標かつ
		coll1.right > coll2.left &&		//右辺のＸ座標＞　左辺のＸ座標かつ
		coll1.top < coll2.bottom &&		//上辺のＹ座標＜下辺のＹ座標かつ
		coll1.bottom > coll2.top		//下辺のＹ座標＞上辺のＹ座標
		)
	{
		//あってるとき
		return TRUE;
	}
	else
	{
		//あってないとき
		return FALSE;
	}
}
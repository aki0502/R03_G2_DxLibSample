//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使うときは必要

//マクロ定義
#define GAME_TITLE	"ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH	1280				//ゲーム画面の幅(ウィドス)
#define GAME_HEIGHT	720					//ゲーム画面の高さ(ハイト)
#define GAME_COLOR  32					//ゲームの色域

#define GAME_ICON_ID	333				//ゲームのICONのID

#define GAME_WINDOW_BAR 0				//ウィンドウバー


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
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする


	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//四角の位置を決める
	int X = GAME_WIDTH / 2;			//横の中心
	int Y = GAME_HEIGHT / 2;		//縦の中心

	//四角の大きさを決める
	int width = 32;		//幅
	int height = 32;	//高さ


	//円の半径を決める
	int radius = 100;

	//速度
	int speed = 10;
	int Xspeed = speed;
	int Yspeed = speed;

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1)
	{
		//何かしらキーが押されたとき
		if (CheckHitKeyAll() != 0)	//0のときに、何かキーが押された
		{
			break;	//無限ループを抜ける
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられた
		{
			break;	//無限ループを抜ける
		}

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//四角を描画
		//DrawBox(
		//	X, Y, X+ width, Y+height,
		//	GetColor(255, 0, 0),		//色を取得
		//	TRUE						//塗りつぶし？
		//);

		//円を描画
		DrawCircle(X, Y, radius, GetColor(0, 255, 0), FALSE, 5);

		X += Xspeed;				//四角の位置を右にずらす
		Y += Yspeed;				//四角の位置を下にずらす

		//質問①　四角を斜め右下に動かそう！
					//heightの左にY+。Y++で下にずらす。

		//質問②　四角を画面の端に来たら、移動する向きを反転させてみよう
		if (X - radius < 0|| X + radius > GAME_WIDTH)	//横画面から出たとき
		{
			//移動速度の符号を反転させる
			//+1なら、-1	-1なら+1
			Xspeed = -Xspeed;			//移動する向きを反転

			//壁に当たったら早くなる
			if (Xspeed > 0) { Xspeed += 2; }
			else if (Xspeed < 0) { Xspeed -= 2; }

			//

		}

		if (Y - radius< 0 || Y + radius > GAME_HEIGHT)	//縦画面から出たとき
		{
			//移動速度の符号を反転させる
			//+1なら、-1	-1なら+1
			Yspeed = -Yspeed;			//移動する向きを反転

			if (Yspeed > 0) { Yspeed += 2; }
			else if (Yspeed < 0) { Yspeed -= 2; }
		}

		ScreenFlip();					//ダブルバッファリングした画面を描画
	}







	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}
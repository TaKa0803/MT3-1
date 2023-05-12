#include <Novice.h>
#include<Vector3.h>
#include<math.h>
#include<Matrix4x4.h>
#include<Vector3Math.h>
#include<renderingPipeline.h>

const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.f, 160.f, 200.0f, 300.0f, 0.0f, 1000.0f);

	Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);

	Matrix4x4 viewportMatrix = MakeViewPortMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		
		MatrixScreenPrintf(0, 0, orthographicMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5, perspectiveFovMatrix);
		MatrixScreenPrintf(0, kRowHeight * 10, viewportMatrix);

		
		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

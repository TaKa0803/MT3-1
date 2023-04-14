#include <Novice.h>
#include<Vector3.h>
#include<math.h>
//加算
Vector3 Add(Vector3 v1, Vector3 v2) {
	Vector3 Answer={
		v1.x+v2.x,
		v1.y+v2.y,
		v1.z+v2.z,
	};
	return Answer;
}
//減算
Vector3 Subtract(Vector3 v1, Vector3 v2) {
	Vector3 Answer = {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z,
	};
	return Answer;
}
//スカラー倍
Vector3 Multipy(float scalar,Vector3 v) {
	Vector3 Answer = {
		v.x*scalar ,
		v.y *scalar,
		v.z *scalar,
	};
	return Answer;
}
//内積
float Dot(Vector3 v1, Vector3 v2) {
	float Answer = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return Answer;
}

//長さ
float Length(Vector3 v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return length;
}

//正規化
Vector3 Normalize(Vector3 v) {
	float length = Length(v);
	Vector3 Answer = {
		v.x / length,
		v.y / length,
		v.z / length,
	};
	return Answer;
}

static const int kCoolumnWidth = 60;

void VectorScreenPrintf(int x, int y, Vector3 v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x + kCoolumnWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x + kCoolumnWidth * 2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x + kCoolumnWidth * 3, y, "%s", label);
}




const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

	
		Vector3 v1 = { 1.0f,3.0f,-5.0f };
		Vector3 v2 = { 4.0f,-1.0f,2.0f };
		float k={ 4.0f };


		Vector3 resultAdd = Add(v1, v2);
		Vector3 resultSubtract = Subtract(v1, v2);
		Vector3 resultMultipy = Multipy(k, v1);
		float resultDot =Dot(v1, v2);
		float resultLength = Length(v1);
		Vector3 resultNormalize = Normalize(v2);

		int kRowHeight = 20;

		VectorScreenPrintf(0, 0, resultAdd, ":Add");
		VectorScreenPrintf(0, kRowHeight, resultSubtract, ":Subtract");
		VectorScreenPrintf(0, kRowHeight * 2, resultMultipy, ":Multiply");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%.02f:Dot", resultDot);
		Novice::ScreenPrintf(0, kRowHeight * 4, "%.02f:Length", resultLength);
		
		VectorScreenPrintf(0, kRowHeight * 5, resultNormalize, ":Normalize");

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

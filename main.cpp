#include <Novice.h>
#include<Vector3.h>
#include<math.h>
#include<Matrix4x4.h>

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

#pragma region 00_02内容
	/*
	Matrix4x4 m1 = {
		3.2f,0.7f,9.6f,4.4f,
		5.5f,1.3f,7.8f,2.1f,
		6.9f,8.0f,2.6f,1.0f,
		0.5f,7.2f,5.1f,3.3f,
	};

	Matrix4x4 m2 = {
		4.1f,6.5f,3.3f,2.2f,
		8.8f,0.6f,9.9f,7.7f,
		1.1f,5.5f,6.6f,0.0f,
		3.3f,9.9f,8.8f,2.2f,
	};

	Matrix4x4 resultAdd = Add(m1, m2);
	Matrix4x4 resultMultiply = Multiply(m1, m2);
	Matrix4x4 resultSubtract = Subtract(m1, m2);
	Matrix4x4 inverseM1 = Inverse(m1);
	Matrix4x4 inverseM2 = Inverse(m2);
	Matrix4x4 tansposeM1 = Transpose(m1);
	Matrix4x4 tansposeM2 = Transpose(m2);

	Matrix4x4 identity = MakeIdentity4x4();
	*/
#pragma endregion

	
	Vector3 translate{ 4.1f,2.6f,0.8f };
	Vector3 scale{ 1.5f,5.2f,7.3f };
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Vector3 point{ 2.3f,3.8f,1.4f };
	Matrix4x4 transformMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};
	Vector3 transformed = Transform(point ,transformMatrix);
	
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*
		MatrixScreenPrintf(0, 0, resultAdd);
		MatrixScreenPrintf(0, kRowHeight * 5, resultSubtract);
		MatrixScreenPrintf(0, kRowHeight * 5*2, resultMultiply);
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, inverseM1);
		MatrixScreenPrintf(0, kRowHeight * 5 * 4, inverseM2);
		MatrixScreenPrintf(kColumnWidth * 5,0, tansposeM1);

		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5, tansposeM2);
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5 * 2, identity);
		*/

		VectorScreenPrintf(0, 0, transformed,"transformed");
		MatrixScreenPrintf(0, kRowHeight, translateMatrix);
		MatrixScreenPrintf(0, kRowHeight * 6, scaleMatrix);
		
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

#include <Novice.h>
#include<Vector3.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<Matrix4x4.h>
#include<Vector3Math.h>
#include<renderingPipeline.h>
#include<stdint.h>


const char kWindowTitle[] = "学籍番号";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;


Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return{
		v1.y*v2.z-(v1.z*v2.y),
		(v1.z*v2.x)-(v1.x*v2.z),
		v1.x*v2.y-(v1.y*v2.x)
	};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 kLocalVertices[3] = {
		{0,10,0},
		{-10,-10,0},
		{10,-10,0},
	};
	Vector3 cameraposition = { 0.0f,0.0f,-100.0f };
	Vector3 scale = { 1.0f,1.0f,1.0f };

	Vector3 rotate = { 0,0,0 };

	Vector3 translate = { 0,0,0 };

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);
	

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		if (keys[DIK_W]) {
			translate.z += 6;
		}
		if (keys[DIK_S]) {
			translate.z += -6;
		}
		if (keys[DIK_A]) {
			translate.x += -5;
		}
		if (keys[DIK_D]) {
			translate.x += 5;
		}
		
		rotate.y += (1.0f/60.0f)*(float)M_PI;

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraposition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		//WVPMatrixをつくる
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		//ViewportMatrixをつくる
		Matrix4x4 viewportMatrix = MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);



		//Screen空間へと頂点を変換する
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; i++) {
			//NDCまで変換。Transformを使うと同時座標->デカルト座標系の処理が行われ、結果的にZDivideが行われることになる
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);

		}

		Novice::DrawTriangle((int)screenVertices[0].x, (int)screenVertices[0].y, (int)screenVertices[1].x, (int)screenVertices[1].y, (int)screenVertices[2].x, (int)screenVertices[2].y, RED, kFillModeSolid);
		
		VectorScreenPrintf(0, 0, cross, "Cross");

		
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

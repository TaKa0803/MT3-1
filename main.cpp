#include <Novice.h>
#include<Vector3.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<Matrix4x4.h>
#include<Vector3Math.h>
#include<renderingPipeline.h>
#include<stdint.h>
#include"ImGuiManager.h"

const char kWindowTitle[] = "学籍番号";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

struct Sphere {
	Vector3 center;	//中心点
	float radius;	//半径
};

struct Segment {
	Vector3 origin;	//始点
	Vector3 diff;	//終点への差分ベクトル
};

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return{
		v1.y*v2.z-(v1.z*v2.y),
		(v1.z*v2.x)-(v1.x*v2.z),
		v1.x*v2.y-(v1.y*v2.x)
	};
}

void DrawGrid(const Matrix4x4& ProjectionMatrix, const Matrix4x4& viewportMatrix,const Matrix4x4&viewMatrix) {
	const float kGridHalfWidth = 2.0f;											//Gridの半分の幅
	const uint32_t kSubdivision = 10;											//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);		//１つ分の長さ
	
	//x方向に変更
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 scale(1.0f, 1.0f, 1.0f);
		Vector3 rotate(0, 0, 0);

		float x = (float)xIndex - kSubdivision / 2;

		Vector3 localS((float)x*kGridEvery, 0,-kGridHalfWidth);
		Vector3 localE((float)x * kGridEvery, 0,kGridHalfWidth);

		

		Matrix4x4 worldS = MakeAffineMatrix(scale,rotate,localS);
		Matrix4x4 worldE =MakeAffineMatrix(scale, rotate,localE);

		Matrix4x4 worldViewProjectionMatrixS = Multiply(worldS, Multiply(viewMatrix, ProjectionMatrix));
		Matrix4x4 worldViewProjectionMatrixE = Multiply(worldE, Multiply(viewMatrix, ProjectionMatrix));


		Vector3 ndcStart = Transform(localS,worldViewProjectionMatrixS);
		Vector3 ndcEnd = Transform(localE, worldViewProjectionMatrixE);

		Vector3 start = Transform(ndcStart, viewportMatrix);
		Vector3 end = Transform(ndcEnd, viewportMatrix);

		//スクリーン座標系まで変換をかける
		//変換した座標を使って表示。色は薄い灰色（0xAAAAAAFF),原点は黒いぐらいがいいがなんでもいい
		if (x == 0) {
			Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, BLACK);
		}
		else {
			Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, WHITE);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		//上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 scale(1.0f, 1.0f, 1.0f);
		Vector3 rotate(0, 0, 0);

		float z = (float)zIndex -kSubdivision/2;

		Vector3 localS(kGridHalfWidth,0,(float)z * kGridEvery);
		Vector3 localE(-kGridHalfWidth,0,(float)z * kGridEvery);

		Matrix4x4 worldS = MakeAffineMatrix(scale, rotate, localS);
		Matrix4x4 worldE = MakeAffineMatrix(scale, rotate, localE);

		Matrix4x4 worldViewProjectionMatrixS = Multiply(worldS, Multiply(viewMatrix, ProjectionMatrix));
		Matrix4x4 worldViewProjectionMatrixE = Multiply(worldE, Multiply(viewMatrix, ProjectionMatrix));


		Vector3 ndcStart = Transform(localS, worldViewProjectionMatrixS);
		Vector3 ndcEnd = Transform(localE, worldViewProjectionMatrixE);

		Vector3 start = Transform(ndcStart, viewportMatrix);
		Vector3 end = Transform(ndcEnd, viewportMatrix);

		//スクリーン座標系まで変換をかける
		//変換した座標を使って表示。色は薄い灰色（0xAAAAAAFF),原点は黒いぐらいがいいがなんでもいい
		if (z == 0) {
			Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, BLACK);
		}
		else {
			Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, WHITE);
		}
	}
}

void DrawPhere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivition = 16;		//bunkatu 
	const float kLonEvery = (float)M_PI*2 * (1.0f / (float)kSubdivition); 
	const float kLatEvery = (float)M_PI * (1.0f / (float) kSubdivition);

	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivition; ++latIndex) {
		float lat = -(float)M_PI/2.0f+ kLatEvery * latIndex;	//現在の情報
		//軽度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivition; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			//
			Vector3 a, b, c;
			a = Scalar(sphere.radius,{ cosf(lat)*cosf(lon),sinf(lat),cosf(lat)*sinf(lon)});
			a = Add(sphere.center,a);
			b = Scalar(sphere.radius, { cosf(lat+kLatEvery) * cosf(lon),sinf(lat+kLatEvery),cosf(lat+kLatEvery) * sinf(lon) });
			b = Add(sphere.center,b);
			c = Scalar(sphere.radius, { cosf(lat) * cosf(lon+kLonEvery),sinf(lat),cosf(lat) * sinf(lon + kLonEvery) });
			c = Add(sphere.center,c);
			
			Vector3 ndcVertex = Transform(a,viewProjectionMatrix);
			a = Transform(ndcVertex, viewportMatrix);

			ndcVertex = Transform(b, viewProjectionMatrix);
			b = Transform(ndcVertex, viewportMatrix);

			ndcVertex = Transform(c, viewProjectionMatrix);
			c = Transform(ndcVertex, viewportMatrix);

			Novice::DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, color);
			Novice::DrawLine((int)a.x, (int)a.y, (int)c.x, (int)c.y, color);
		}
	}


}


Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float v22 = Length(v2);
	float t = dot / (v22 * v22);
	return Scalar(t, v2);
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	return Add(segment.origin, project);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	
	Vector3 cameraposition = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,0 };

	
	Sphere sphere{
		{0,0},
		1,
	};

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		

		float camerapos[3] = {cameraposition.x,cameraposition.y,cameraposition.z};
		float cameraR[3] = { cameraRotate.x,cameraRotate.y,cameraRotate.z };
		cameraposition = { camerapos[0],camerapos[1],camerapos[2] };
		cameraRotate = { cameraR[0],cameraR[1],cameraR[2] };
		
		rotate.y += (1.0f/60.0f)*(float)M_PI;

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f },cameraRotate, cameraposition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		//ViewportMatrixをつくる
		Matrix4x4 viewportMatrix = MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		
		//WVPMatrixをつくる
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		
		
		
		
		DrawGrid(projectionMatrix,viewportMatrix,viewMatrix);


		//円のWVP作成
		//Matrix4x4 SphereworldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0,0,0 }, sphere.center);
		//Matrix4x4 SphereviewProjection = Multiply(SphereworldMatrix, Multiply(viewMatrix, projectionMatrix));
		//DrawPhere(sphere, SphereviewProjection, viewportMatrix, WHITE);

		Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
		ImGui::InputFloat3("project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		Vector3 closestPoint = ClosestPoint(point, segment);


		Sphere pointSphere{ point,0.01f };
		Sphere closestPointSphere{ closestPoint,0.01f };

		Matrix4x4 pointworld = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0,0,0 }, pointSphere.center);
		Matrix4x4 pointWVP = Multiply(pointworld, Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 pointworldE = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0,0,0 }, closestPointSphere.center);
		Matrix4x4 pointWVPE = Multiply(pointworldE, Multiply(viewMatrix, projectionMatrix));

		DrawPhere(pointSphere,pointWVP, viewportMatrix, RED);
		DrawPhere(closestPointSphere, pointWVPE, viewportMatrix, BLACK);



		
		//線分の描画
		Matrix4x4 lineworld = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0,0,0 }, segment.origin);
		Matrix4x4 lineViewprojectM = Multiply(lineworld, Multiply(viewMatrix, projectionMatrix));
		Vector3 start = Transform(Transform(segment.origin,lineViewprojectM), viewportMatrix);
		
		Matrix4x4 lineworldE = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0,0,0 }, Add(segment.origin,segment.diff));
		Matrix4x4 lineViewprojectME = Multiply(lineworldE, Multiply(viewMatrix, projectionMatrix));
		Vector3 end = Transform(Transform(Add(segment.origin,segment.diff),lineViewprojectME), viewportMatrix);
		
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);


		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraposition.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

		//Novice::DrawTriangle((int)screenVertices[0].x, (int)screenVertices[0].y, (int)screenVertices[1].x, (int)screenVertices[1].y, (int)screenVertices[2].x, (int)screenVertices[2].y, RED, kFillModeSolid);
		
		
		
		
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

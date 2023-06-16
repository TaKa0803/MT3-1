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
	unsigned int color;//色
};

struct Segment {
	Vector3 origin;	//始点
	Vector3 diff;	//終点への差分ベクトル
};

struct Plane {
	Vector3 normal;
	float distance;
	unsigned int color;
};



Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return{
		v1.y*v2.z-(v1.z*v2.y),
		(v1.z*v2.x)-(v1.x*v2.z),
		v1.x*v2.y-(v1.y*v2.x)
	};
}

Vector3 WorldtoScreen(Vector3 vec, const Matrix4x4& view, const Matrix4x4& viewport, const Matrix4x4& projection) {
	Matrix4x4 world = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, vec);
	Matrix4x4 WVP = Multiply(world, Multiply(view, projection));
	Vector3 ndc = Transform(vec, WVP);
	return Transform(ndc, viewport);
}

void DrawGrid2(const Matrix4x4& view, const Matrix4x4& vieport, const Matrix4x4& projection) {
	const float kGridHalfWidth = 2.0f;										//Grid半分の幅
	const uint32_t kSubdivition = 10;										//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivition);	//１つ分の長さ
	//奥から手前への線を順々に引いていく
	
	for (uint32_t xIndex = 0; xIndex <= kSubdivition; ++xIndex) {
		//上の情報を使ってワールド座標系上の始点と終点を求める
		float z = (float)xIndex - kSubdivition / 2.0f;
		Vector3 st, ed;
		st = { -kGridHalfWidth,0,z * kGridEvery };
		ed = { +kGridHalfWidth,0,z * kGridEvery };
		//スクリーン座標系まで変換をかける
		Vector3 Sst, Sed;
		Sst = WorldtoScreen(st, view, vieport, projection);
		Sed = WorldtoScreen(ed, view, vieport, projection);
		//変換した座標を使って表示。色は薄い灰色（0xAAAAAAFF),原点は黒ぐらいがいいがなんでもいい
		if (z == 0) {
			Novice::DrawLine((int)Sst.x, (int)Sst.y, (int)Sed.x, (int)Sed.y, BLACK);
		}
		else {
			Novice::DrawLine((int)Sst.x, (int)Sst.y, (int)Sed.x, (int)Sed.y, WHITE);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivition; ++zIndex) {
		//
		float x = (float)zIndex - kSubdivition / 2.0f;
		Vector3 st, ed;
		st = { x * kGridEvery ,0,-kGridHalfWidth};
		ed = { x * kGridEvery ,0,kGridHalfWidth};
		//スクリーン座標系まで変換をかける
		Vector3 Sst, Sed;
		Sst = WorldtoScreen(st, view, vieport, projection);
		Sed = WorldtoScreen(ed, view, vieport, projection);
		//変換した座標を使って表示。色は薄い灰色（0xAAAAAAFF),原点は黒ぐらいがいいがなんでもいい
		if (x == 0) {
			Novice::DrawLine((int)Sst.x, (int)Sst.y, (int)Sed.x, (int)Sed.y, BLACK);
		}
		else {
			Novice::DrawLine((int)Sst.x, (int)Sst.y, (int)Sed.x, (int)Sed.y, WHITE);
		}
	}
	
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& view, const Matrix4x4& viewport,const Matrix4x4&projection ) {
	const uint32_t kSubdivition = 20;										//分割数
	const float kLonEvery = (float)M_PI * 2 * (1.0f / (float)kSubdivition); //軽度分割１つ分の角度
	const float kLatEvery = (float)M_PI * (1.0f / (float)kSubdivition);		//緯度分割一つ分の角度
	//緯度の方向に分割　-Π/2 ~Π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivition; ++latIndex) {
		float lat =-(float)M_PI / 2.0f + kLatEvery * latIndex;				//現在の緯度
		//経度の方向に分割0~2Π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivition; ++lonIndex) {
			float lon = lonIndex * kLonEvery;								//現在の経度
			//world座標系でのabcを求める
			Vector3 a, b, c;
			a = Scalar(sphere.radius, { cos(lat) * cos(lon),sin(lat),cos(lat) * sin(lon) });
			b = Scalar(sphere.radius, { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery),cos(lat + kLatEvery) * sin(lon) });
			c = Scalar(sphere.radius, { cos(lat) * cos(lon + kLonEvery),sin(lat),cos(lat) * sin(lon + kLonEvery) });
			
			a = Add(a, sphere.center);
			b = Add(b, sphere.center);
			c = Add(c, sphere.center);

			
			//a,b,cをScreen座標系まで変換
			Vector3 Sa, Sb, Sc;
			Sa = WorldtoScreen(a, view, viewport, projection);
			Sb = WorldtoScreen(b, view, viewport, projection);
			Sc = WorldtoScreen(c, view, viewport, projection);

			//ab,acで線を引く
			Novice::DrawLine((int)Sa.x, (int)Sa.y, (int)Sb.x, (int)Sb.y, sphere.color);
			Novice::DrawLine((int)Sa.x, (int)Sa.y, (int)Sc.x, (int)Sc.y, sphere.color);
		}
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewprojectionM, const Matrix4x4& viewportM) {
	const uint32_t kSubdivition = 20;										//分割数
	const float kLonEvery = (float)M_PI * 2 * (1.0f / (float)kSubdivition); //軽度分割１つ分の角度
	const float kLatEvery = (float)M_PI * (1.0f / (float)kSubdivition);		//緯度分割一つ分の角度

	//緯度の方向に分割　-Π/2 ~Π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivition; ++latIndex) {
		float lat = -(float)M_PI / 2.0f + kLatEvery * latIndex;				//現在の緯度
		//経度の方向に分割0~2Π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivition; ++lonIndex) {
			float lon = lonIndex * kLonEvery;								//現在の経度
			//world座標系でのabcを求める
			Vector3 a, b, c;
			a = Scalar(sphere.radius, { cos(lat) * cos(lon),sin(lat),cos(lat) * sin(lon) });
			b = Scalar(sphere.radius, { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery),cos(lat + kLatEvery) * sin(lon) });
			c = Scalar(sphere.radius, { cos(lat) * cos(lon + kLonEvery),sin(lat),cos(lat) * sin(lon + kLonEvery) });

			a = Add(a, sphere.center);
			b = Add(b, sphere.center);
			c = Add(c, sphere.center);


			//a,b,cをScreen座標系まで変換
			Vector3 Sa, Sb, Sc;
			Sa = Transform(Transform(a, viewprojectionM), viewportM);
			Sb = Transform(Transform(b, viewprojectionM), viewportM);
			Sc = Transform(Transform(c, viewprojectionM), viewportM);

			//ab,acで線を引く
			Novice::DrawLine((int)Sa.x, (int)Sa.y, (int)Sb.x, (int)Sb.y, sphere.color);
			Novice::DrawLine((int)Sa.x, (int)Sa.y, (int)Sc.x, (int)Sc.y, sphere.color);
		}
	}
}

bool InCollision(const Sphere& s1, const Sphere& s2) {
	Vector3 a= {
		s1.center.x-s2.center.x,
		s1.center.y - s2.center.y,
		s1.center.z - s2.center.z,
	};
	float Hlong = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	if (Hlong <=( s1.radius + s2.radius)) {
		return true;
	}
	return false;
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

bool InCollision(const Sphere& sphere, const Plane& plane) {
	Vector3 v1 = sphere.center;
	Vector3 v2 = Scalar(plane.distance, plane.normal);
	if (Length(Subtract(v1, v2)) <= sphere.radius) {
		return true;
	}
	return false;
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return{ 0.0f,-vector.z,vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewprojectionM, const Matrix4x4& viewportM) {
	Vector3 center = Scalar(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = {-perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z};
	perpendiculars[2] = Cross(plane.normal,perpendiculars[0]);
	perpendiculars[3] = {perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z};

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Scalar(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewprojectionM), viewportM);
	}
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, plane.color);
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[3].x, (int)points[3].y, plane.color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, plane.color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, plane.color);

}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,0 };

	
	Sphere sphere1{
		{0,0},
		1,
		WHITE,
	};
	Plane plane{
		{0,1,0},
		2,
		WHITE
	};
	

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere1.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere1.radius, 0.01f);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane distance", &plane.distance, 0.01f);
		ImGui::End();
		plane.normal = Normalize(plane.normal);

		if (InCollision(sphere1, plane)) {
			sphere1.color = RED;
			plane.color = RED;
		}
		else {
			sphere1.color = WHITE;
			plane.color = WHITE;
		}
		
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f },cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewportMatrix = MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Matrix4x4 vpMatrix = Multiply(viewMatrix, projectionMatrix);	
		
		DrawGrid2(viewMatrix, viewportMatrix, projectionMatrix);
		
		//DrawSphere(sphere1, viewMatrix, viewportMatrix, projectionMatrix);
		DrawSphere(sphere1, vpMatrix, viewportMatrix);
		DrawPlane(plane, vpMatrix, viewportMatrix);
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

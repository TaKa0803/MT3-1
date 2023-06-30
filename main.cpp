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

struct Triangle{
	Vector3 verties[3];//頂点
	unsigned int color;
};

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return{
		v1.y*v2.z-(v1.z*v2.y),
		(v1.z*v2.x)-(v1.x*v2.z),
		v1.x*v2.y-(v1.y*v2.x)
	};
}

void DrawLine(const Segment& segment, const Matrix4x4& viewprojectionM, const Matrix4x4& viewport) {
	Vector3 st, ed;
	st = Transform(Transform(segment.origin, viewprojectionM), viewport);
	ed = Transform(Transform(Add(segment.origin,segment.diff), viewprojectionM), viewport);
	Novice::DrawLine((int)st.x, (int)st.y, (int)ed.x, (int)ed.y, WHITE);
}
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewprojectionM, const Matrix4x4& viewport) {
	Vector3 reP[3];
	for (int i = 0; i < 3; i++) {
		reP[i] = Transform(Transform(triangle.verties[i], viewprojectionM), viewport);
	}
	Novice::DrawLine((int)reP[0].x, (int)reP[0].y, (int)reP[1].x, (int)reP[1].y, triangle.color);
	Novice::DrawLine((int)reP[0].x, (int)reP[0].y, (int)reP[2].x, (int)reP[2].y, triangle.color);
	Novice::DrawLine((int)reP[2].x, (int)reP[2].y, (int)reP[1].x, (int)reP[1].y, triangle.color);

}

void DrawGrid(const Matrix4x4& viewprojectionM, const Matrix4x4& viewport) {
	const float kGridHalfWidth = 4.0f;										//Grid半分の幅
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
		Sst = Transform(Transform(st, viewprojectionM), viewport);
		Sed = Transform(Transform(ed, viewprojectionM), viewport);

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
		st = { x * kGridEvery ,0,-kGridHalfWidth };
		ed = { x * kGridEvery ,0,kGridHalfWidth };
		//スクリーン座標系まで変換をかける
		Vector3 Sst, Sed;
		Sst = Transform(Transform(st, viewprojectionM), viewport);
		Sed = Transform(Transform(ed, viewprojectionM), viewport);

		//変換した座標を使って表示。色は薄い灰色（0xAAAAAAFF),原点は黒ぐらいがいいがなんでもいい
		if (x == 0) {
			Novice::DrawLine((int)Sst.x, (int)Sst.y, (int)Sed.x, (int)Sed.y, BLACK);
		}
		else {
			Novice::DrawLine((int)Sst.x, (int)Sst.y, (int)Sed.x, (int)Sed.y, WHITE);
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

//円と円
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
//円と面
bool InCollision(const Sphere& sphere, const Plane& plane) {
	Vector3 v1 = sphere.center;
	Vector3 v2 = Scalar(plane.distance, plane.normal);
	if (Length(Subtract(v1, v2)) <= sphere.radius) {
		return true;
	}
	return false;
}
//線と面
bool InCollision(const Segment& segment, const Plane& plane) {
	//法線と線の内積
	float dot = Dot(plane.normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}
	else {
		//tを求める
		float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
		
		Vector3 a = Add(segment.origin, Scalar(t, segment.diff));
		
		if (a.x >= segment.origin.x && a.x <= segment.origin.x + segment.diff.x && a.y >= segment.origin.y && a.y <= segment.origin.y + segment.diff.y && a.z >= segment.origin.z && a.z <= segment.origin.z + segment.diff.z) {
			return true;
		}
		
	}
	return false;
}
//線と三角形
bool InCollision(const Segment& segment, const Triangle& triangle) {
	//三角形の法線
	Vector3 AB = Subtract(triangle.verties[1], triangle.verties[0]);
	Vector3 AC = Subtract(triangle.verties[2], triangle.verties[0]);
	Vector3 normal = Normalize(Cross(AB, AC));
	//線と三角形の法線の内積
	float dot = Dot(normal,segment.diff);
	if (dot == 0.0f) {
		return false;
	}
	else {
		//tを求める
		float t(0 -Dot(segment.origin, normal) / dot);

		Vector3 a = Add(segment.origin, Scalar(t, segment.diff));

		//線の接点が線分内にあるなら
		if (a.x >= segment.origin.x && a.x <= segment.origin.x + segment.diff.x && a.y >= segment.origin.y && a.y <= segment.origin.y + segment.diff.y && a.z >= segment.origin.z && a.z <= segment.origin.z + segment.diff.z) {
			Vector3 v01 = Subtract(triangle.verties[1], triangle.verties[0]);
			Vector3 v12 = Subtract(triangle.verties[2], triangle.verties[1]);
			Vector3 v20 = Subtract(triangle.verties[0], triangle.verties[2]);
			
			Vector3 v1p = Subtract(a, triangle.verties[1]);
			Vector3 v2p = Subtract(a, triangle.verties[2]);
			Vector3 v0p = Subtract(a, triangle.verties[0]);

			Vector3 ross01 = Cross(v01, v0p);
			Vector3 ross12 = Cross(v12, v1p);
			Vector3 ross20 = Cross(v20, v2p);

			if (Dot(ross01, normal) >= 0.0f && Dot(ross12, normal) >= 0.0f && Dot(ross20, normal) >= 0.0f) {
				return true;
			}
		}
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
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return{ 0.0f,-vector.z,vector.y };
}



struct Transformation {
	Vector3 translate;
	Vector3 rotate;
	Vector3 scale;
};

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


void CameraEdit(Transformation& camera, char key[]) {
	//原点からカメラへのベクトル
	Vector3 tocamera = Normalize(Subtract(camera.translate,{0,0,0}));
	camera.translate =Add(camera.translate,Scalar((float)Novice::GetWheel()/100, tocamera));
	
	if (key[DIK_UP]) {

	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	
	
	Transformation camera{
		{0.0f,1.9f,-6.49f},
		{0.26f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
	};


	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,0 };

	Segment line{
		{0,0,0},
		{1,1,0}
	};
	

	Triangle triangle;
	
	triangle.verties[0] = { 0,2,0 };
	triangle.verties[1] = { -2,0,0 };
	triangle.verties[2] = { 2,0,0 };

	
	

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		

		CameraEdit(camera,keys);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &camera.translate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &camera.rotate.x, 0.01f);
		ImGui::DragFloat3("CameraScale", &camera.scale.x, 0.01f);
		ImGui::DragFloat3("line Origion", &line.origin.x, 0.01f);
		ImGui::DragFloat3("line diff", &line.diff.x, 0.01f);

		ImGui::End();
		
		
		
		Matrix4x4 cameraMatrix = MakeAffineMatrix(camera.scale,camera.rotate,camera.translate);

		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewportMatrix = MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Matrix4x4 vpMatrix = Multiply(viewMatrix, projectionMatrix);	
		
		if (InCollision(line, triangle)) {
			triangle.color = RED;
		}
		else {
			triangle.color = WHITE;
		}

		DrawGrid(vpMatrix, viewportMatrix);
		DrawLine(line, vpMatrix, viewportMatrix);

		DrawTriangle(triangle, vpMatrix, viewportMatrix);
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

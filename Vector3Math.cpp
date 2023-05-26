#include<Vector3Math.h>
#include<math.h>
#include<Novice.h>
//加算
Vector3 Add(Vector3 v1, Vector3 v2) {
	Vector3 Answer = {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z,
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
Vector3 Scalar(float scalar, Vector3 v) {
	Vector3 Answer = {
		v.x * scalar ,
		v.y * scalar,
		v.z * scalar,
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

//ベクトル描画
void VectorScreenPrintf(int x, int y, Vector3 v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x + kCoolumnWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x + kCoolumnWidth * 2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x + kCoolumnWidth * 3, y, "%s", label);
}
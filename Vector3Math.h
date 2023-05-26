#pragma once
#include<Vector3.h>

//加算
Vector3 Add(Vector3 v1, Vector3 v2);
//減算
Vector3 Subtract(Vector3 v1, Vector3 v2);
//スカラー倍
Vector3 Scalar(float scalar, Vector3 v);
//内積
float Dot(Vector3 v1, Vector3 v2);

//長さ
float Length(Vector3 v);

//正規化
Vector3 Normalize(Vector3 v);

static const int kCoolumnWidth = 60;
//ベクトル描画
void VectorScreenPrintf(int x, int y, Vector3 v, const char* label);
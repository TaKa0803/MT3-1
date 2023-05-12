#pragma once
#include<Matrix4x4.h>
#include<Vector3.h>

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//正射投影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

//ビューポート変換
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

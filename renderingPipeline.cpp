#include<renderingPipeline.h>
#include<cmath>



//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	return {
		(1.0f/aspectRatio)*(1.0f/std::tan(fovY/2.0f)),	0,							0,							0,
		0,												(1.0f/std::tan(fovY/2.0f)),	0,							0,
		0,												0,							(farClip/(farClip-nearClip)),1,
		0,												0,							(-nearClip*farClip/(farClip-nearClip)),0
	};
}


//正射投影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	return {
		2.0f/(right-left),0,0,0,
		0,2.0f/(top-bottom),0,0,
		0,0,1.0f/(farClip-nearClip),0,
		(left+right)/(left-right),(top+bottom)/(bottom-top),nearClip/(nearClip-farClip),1,
	};
}

//ビューポート変換
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width/2.0f,		0,					0,					0,
		0,				-(height/2.0f),		0,					0,
		0,				0,					maxDepth-minDepth,	0,
		left+(width/2.0f),top+(height/2.0f),minDepth,			1
	};
}


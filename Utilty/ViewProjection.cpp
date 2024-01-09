#include "ViewProjection.h"


ViewProjection::~ViewProjection()
{
	
}

void ViewProjection::Initialize()
{
	engine_ = MyEngine::GetInstance();
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void ViewProjection::CreateConstBuffer()
{
	constBuff_ = engine_->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
}

void ViewProjection::Map() {
	constBuff_->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void ViewProjection::UpdateMatrix() 
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void ViewProjection::TransferMatrix() 
{
	constMap->view = matView;
	constMap->projection = matProjection;
}

void ViewProjection::UpdateViewMatrix()
{
	Matrix4x4 cameraMatrix = MakeAffinMatrix({ 1.0f,1.0f,1.0f }, rotation_, translation_);
	matView = Inverse(cameraMatrix);
}

void ViewProjection::UpdateProjectionMatrix()
{
	matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);
}

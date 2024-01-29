#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
class Titlename
{
public:
	void Initialize(Model* model, Vector3 translation, const std::string& filename);
	~Titlename();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorldTransform() { return worldTransform_.translation_; }

private:
	// ワールド変換データ0
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
};

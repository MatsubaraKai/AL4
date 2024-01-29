#include "Titlename.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void Titlename::Initialize(Model* model, Vector3 translation, const std::string& filename) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	model_->Initialize("Resources", filename);

	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
}

Titlename::~Titlename()
{

}
void Titlename::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Titlename::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	model_->Draw(viewProjection, worldTransform_);
}

Vector3 Titlename::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

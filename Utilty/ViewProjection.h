#pragma once

#include"MathUtilty.h"
#include"MyEngine.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列
};

/// <summary>
/// ビュープロジェクション変換データ
/// </summary>
struct ViewProjection {

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;


	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0.0f, 0.0f,0.0f };
	// ローカル座標
	Vector3 translation_ = { 0, 0, -70 };

	MyEngine* engine_ = nullptr;

	// 垂直方向視野角
	float fovAngleY = 45.0f * 3.141592654f / 180.0f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;


	// ビュー行列
	Matrix4x4 matView;
	// 射影行列
	Matrix4x4 matProjection;

	~ViewProjection();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();
	/// <summary>
	/// ビュー行列を更新する
	/// </summary>
	void UpdateViewMatrix();
	/// <summary>
	/// 射影行列を更新する
	/// </summary>
	void UpdateProjectionMatrix();
};

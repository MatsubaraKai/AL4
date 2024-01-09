#pragma once
#include"Model.h"
#include"Vector3.h"
#include"Input.h"
#include"Audio.h"
#include<cassert>
#include"WorldTransform.h"
class Player
{
public:
	~Player();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Initialize(Model* model,Vector3 position, const std::string& filename);
	
	/// <summary>
	/// 初期化
	/// </ summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollisionX();
	void OnCollisionY();
	void OnCollisionGoal();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void GamePlayReset();
	void Reset();

	bool GetIsClear()const { return isGoal; };


private:

	Model* model_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Vector2 speed_ = {};
	WorldTransform worldTransform_;

	// キャラクターの移動速さ
	float kCharacterSpeedX = 0.15f;
	float kCharacterSpeedY = 0.15f;

	float kCharacterSpeed = 0.3f;

	bool isStart = false;
	bool isGoal = false;
	bool isOver = false;

	bool isReset;

	Vector3 move;
};


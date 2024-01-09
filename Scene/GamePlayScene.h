#pragma once
#include<Windows.h>
#include"Triangle.h"
#include"ImGuiManager.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Input.h"
#include"Audio.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

#include"Player.h"
#include"Object/Block/Block.h"
#include"CSVReader.h"

class GamePlayScene
{
public:

	~GamePlayScene();
	void Initialize();
	void Update();
	void Draw();


	/// <summary>
///	衝突判定と応答
/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// ブロックデータの読み込み
	/// </summary>
	void LoadBlockPopData();

	/// <summary>
	/// ブロック発生コマンドの更新
	/// </summary>
	void UpdateBlockPopCommands();

	void BlockSpown(Vector3, float);

	/// <summary>
	///	敵:敵弾を追加する
	/// </summary>
	void AddBlock(Block* block);

	bool GetIsClear()const { return IsClear; };
	bool GetIsOver(){ return IsOver; };

	void SetIsOver(bool isOver);

	void Reset();

private:

	TextureManager* texture = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	SoundData soundData1;

	SpriteData* spriteData = nullptr;
	Sprite* sprite;
	Sphere* sphere;
	Model* model_ = nullptr;
	Model* model2_ = nullptr;
	Model* model3_ = nullptr;
	Model* model4_ = nullptr;

	Model* resetModel = nullptr;

	uint32_t uvTexture;
	uint32_t monsterTexture;

	ViewProjection viewProjection_;
	WorldTransform spriteTransform_;
	WorldTransform resetWorldTransform;

	Player* player_ = nullptr;

	bool IsClear;
	bool IsOver;

	Vector3 blockPos;


	// ブロック発生コマンド
	std::stringstream blockPopCommands;
	// ブロック
	std::list<Block*> blocks_;
};


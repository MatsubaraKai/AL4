#pragma once
#include<Windows.h>
#include"WindowAPI.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"Triangle.h"
#include"ImGuiManager.h"
#include"TextureManager.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Input.h"
#include"WorldTransform.h"
#include"ViewProjection.h"


class GameOverScene
{

public:
	~GameOverScene();
	void Initialize();
	void Update();
	void Draw();


private:
	TextureManager* texture = nullptr;
	Input* input = nullptr;

	SpriteData* spriteData = nullptr;
	Sphere* sphere;
	Sprite* sprite;
	Model* model;
	Model* model2;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	uint32_t uvTexture;
	uint32_t monsterTexture;

	Transform transform_;
	Transform transform2_;
};


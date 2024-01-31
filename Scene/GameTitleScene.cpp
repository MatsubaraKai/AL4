#include "GameTitleScene.h"


GameTitleScene::~GameTitleScene()
{
	delete model;
	delete sphere;
}

void GameTitleScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sprite = new Sprite;
	model = new Model;
	sphere = new Sphere;
	model->Initialize("Resources", "gamename.obj");
	uvTexture = texture->LoadTexture("Resources/game2.png");
	sphere->Initialize(uvTexture);
	sprite->Initialize(spriteData, uvTexture, materialData);
	Vector3 position = { 0,10,0 };

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 10,10,0 };
	worldTransform_.UpdateMatrix();

	viewProjection_.Initialize();
}

void GameTitleScene::Update()
{
	sprite->Update();
	input->Update();
	sphere->Update();
	worldTransform_.UpdateMatrix();
}

void GameTitleScene::Draw()
{
	//sprite->Draw(worldTransform_);
	model->Draw(viewProjection_, worldTransform_);
	sphere->Draw();
}


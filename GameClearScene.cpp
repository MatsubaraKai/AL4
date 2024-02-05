#include "GameClearScene.h"

GameClearScene::~GameClearScene()
{
	delete model;
	delete sphere;

}

void GameClearScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();

	model = new Model;
	sphere = new Sphere;
	model->Initialize("Resources", "gameclear.obj");
	uvTexture = texture->LoadTexture("Resources/game2.png");
	sphere->Initialize(uvTexture);

	Vector3 position = { 0,10,0 };

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 10,10,0 };
	worldTransform_.UpdateMatrix();

	viewProjection_.Initialize();




}

void GameClearScene::Update()
{
	input->Update();
	sphere->Update();
	worldTransform_.UpdateMatrix();
}

void GameClearScene::Draw()
{
}


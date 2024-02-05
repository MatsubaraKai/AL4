#include "GamePlayScene.h"


GamePlayScene::~GamePlayScene()
{
	delete model_;
	delete resetModel;
	delete sprite;
	delete spriteData;
	delete sphere;

	for (Block* block : blocks_) {
		delete block;
	}
}

void GamePlayScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();

	spriteData = new SpriteData;
	sprite = new Sprite;
	model_ = new Model;
	resetModel = new Model;
	sphere = new Sphere;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	spriteData->vertex[0] = { 0.0f,360.0f,0.0f,1.0f };
	spriteData->vertex[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData->vertex[2] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData->vertex[3] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData->vertex[4] = { 640.0f,0.0f,0.0f,1.0f };
	spriteData->vertex[5] = { 640.0f,360.0f,0.0f,1.0f };

	spriteData->transform =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	spriteTransform_.Initialize();

	uvTexture = texture->LoadTexture("Resources/game3.png");
	monsterTexture = texture->LoadTexture("Resources/monsterBall.png");
	soundData1 = audio->SoundLoadWave("Resources/Sounds/loop1.wav");
	audio->SoundPlayWave(soundData1, true);

	sprite->Initialize(spriteData, uvTexture);
	sphere->Initialize(uvTexture);
	resetWorldTransform.Initialize();
	resetWorldTransform.translation_ = { 20,-20,0 };
	resetWorldTransform.scale_ = { 5,5,0 };

	resetWorldTransform.UpdateMatrix();
	IsOver = false;
}

void GamePlayScene::Update()
{
	input->Update();
	
	resetWorldTransform.UpdateMatrix();
	sphere->Update();
	sprite->Update();
}

void GamePlayScene::Draw()
{
	sprite->Draw(spriteTransform_);
	sphere->Draw();
}


void GamePlayScene::Reset()
{
	player_->Reset();
}

#pragma once
#include"GamePlayScene.h"
#include"GameTitleScene.h"
#include"GameOverScene.h"
#include"GameClearScene.h"
#include"LeakCheck.h"


enum Scene
{
	TITLE,
	PLAY,
	OVER,
	CLEAR,
};

class GameManager
{
public:

	~GameManager();
	void Initialize();
	void Update();
	void Draw();

private:
	GamePlayScene* gameScene = nullptr;
	GameTitleScene* title = nullptr;
	GameOverScene* over = nullptr;
	GameClearScene* clear = nullptr;

	const wchar_t* kWindowTitle = L"engin";

	//アプリケーションの開始
	const uint32_t kWindowWidth = 1280;
	const uint32_t kWindowHeight = 720;

	WindowAPI* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	MyEngine* engine = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	TextureManager* texture = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	bool xButtonWasPressed = false;

	SoundData toachSE_;

	Scene scene;
	

};

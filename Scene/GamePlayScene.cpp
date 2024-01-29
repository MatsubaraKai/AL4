#include "GamePlayScene.h"


GamePlayScene::~GamePlayScene()
{
	delete player_;
	delete model_;
	delete model2_;
	delete model3_;
	delete model4_;
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
	model2_ = new Model;
	model3_ = new Model;
	model4_ = new Model;
	resetModel = new Model;
	sphere = new Sphere;
	player_ = new Player;

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
	Vector3 playerPos = { -40, -22, 0 };
	player_->Initialize(model_, playerPos, "plane.obj");
	resetModel->Initialize("Resources", "Reset.obj");

	resetWorldTransform.Initialize();
	resetWorldTransform.translation_ = { 20,-20,0 };
	resetWorldTransform.scale_ = { 5,5,0 };

	resetWorldTransform.UpdateMatrix();

	LoadBlockPopData();

	IsOver = false;
}

void GamePlayScene::Update()
{
	input->Update();

	UpdateBlockPopCommands();
	for (Block* block : blocks_) {
		block->Update();
	}
	player_->Update();
	
	resetWorldTransform.UpdateMatrix();

	sphere->Update();
	sprite->Update();

	CheckAllCollisions();

	IsClear = player_->GetIsClear();

}

void GamePlayScene::Draw()
{

	for (Block* block : blocks_) {
		block->Draw(viewProjection_);
	}



	player_->Draw(viewProjection_);
	//sprite->Draw(spriteTransform_);
	sphere->Draw();
}


void GamePlayScene::CheckAllCollisions(){
	// 判定衝突AとBの座標
	Vector3 posA, posB;

#pragma region 自キャラとブロックの当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラとブロックすべての当たり判定
	for (Block* block : blocks_) {
		// ブロックの座標
		posB = block->GetWorldPosition();

		float p2eX = (posB.x - posA.x) * (posB.x - posA.x);
		float p2eY = (posB.y - posA.y) * (posB.y - posA.y);
		float p2eZ = (posB.z - posA.z) * (posB.z - posA.z);

		float pRadius = 1.0f;
		float eRadius = 1.0f;

		float L = (pRadius + eRadius) * (pRadius + eRadius);

		if (p2eX + p2eY + p2eZ <= L) {
			if (block->GetType() == 1) {
				player_->OnCollisionY();
			}

			if (block->GetType() == 2) {
				player_->OnCollisionX();
			}

			if (block->GetType() == 3) {
				player_->OnCollisionGoal();
			}

			if (block->GetType() == 4) {
				IsOver = true;
			}
		}

		/*float leftA = posA.x;
		float rightA = posA.x + 2.0f;
		float topA = posA.y;
		float bottomA = posA.y - 2.0f;
		float nearA = posA.z;
		float farA = posA.z + 2.0f;

		float leftB = posB.x;
		float rightB = posB.x + 2.0f;
		float topB = posB.y;
		float bottomB = posB.y - 2.0f;
		float nearB = posB.z;
		float farB = posB.z + 2.0f;

		if (leftA <= rightB && leftB <= rightA &&
			topA <= bottomB && topB <= bottomA &&
			nearA <= farB && nearB <= farA) {

			if (block->GetType() == 1) {
				player_->OnCollisionY();
			}

			if (block->GetType() == 2) {
				player_->OnCollisionX();
			}
		}*/
	}

#pragma endregion
}

void GamePlayScene::LoadBlockPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("csv/blockPop.csv");
	assert(file.is_open());

	// ファイルも内容を文字列ストリームにコピー
	blockPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdateBlockPopCommands() {

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(blockPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			blockPos.x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			blockPos.y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			blockPos.z = (float)std::atof(word.c_str());
		}

		// TYPEコマンド
		if (word.find("TYPE") == 0) {
			// type
			getline(line_stream, word, ',');
			float type = (float)std::atof(word.c_str());

			// ブロックを発生させる
			BlockSpown(Vector3(blockPos.x, blockPos.y, blockPos.z), type);

			break;
		}
	}
}

void GamePlayScene::BlockSpown(Vector3 translation, float type) {
	// ブロックの生成
 	Block* block_ = new Block();
	// ブロックの初期化
	if (type <= 2) {
		block_->Initialize(model2_,  translation,"cube.obj");
	}
	if (type == 3) {
		block_->Initialize(model3_, translation, "clearblock.obj");
	}
	if (type == 4) {
		block_->Initialize(model4_, translation, "damageblock.obj");
	}
	// ブロックのタイプ設定
	block_->SetType(type);
	AddBlock(block_);
}

void GamePlayScene::AddBlock(Block* block) {
	// リストに登録する
	blocks_.push_back(block);
}

void GamePlayScene::SetIsOver(bool isOver){
	IsOver = isOver;
}

void GamePlayScene::Reset()
{
	player_->Reset();
}

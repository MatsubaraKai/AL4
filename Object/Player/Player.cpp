#include "Player.h"

Player::~Player()
{
	
}

/// <summary>
/// 初期化
/// </ summary>
void Player::Initialize(Model* model, Vector3 position, const std::string& filename)
{
	assert(model);
	model_ = model;

	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_->Initialize("Resources", filename);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	isReset = false;
}

/// <summary>
/// 初期化
/// </ summary>
void Player::Update()
{// キャラクターの移動ベクトル
		move = { 0, 0, 0 };
		//ゲームパットの状態を得る変数(XINPUT)
		XINPUT_STATE joyState;

		// 押した方向で移動ベクトルを変更(左右)
		if (!Input::GetInstance()->GetJoystickState(0, joyState))
		{
			
			if (input_->PushKey(DIK_SPACE) && Jump == false) {
				JumpVelocity = 1.0f;
				Jump = true;
			}

			if (Jump == true) {
				move.y += JumpVelocity -= JumpGravity;
			}
			
			if (input_->PushKey(DIK_D)) {
				{
					move.x += kCharacterSpeed;
				}
			}
			if (input_->PushKey(DIK_A)) {
				{
					move.x -= kCharacterSpeed;
				}
			}
			if (worldTransform_.translation_.y < 2.0f) {
				worldTransform_.translation_.y = 2.0f;
				Jump = false;
			}
			if (worldTransform_.translation_.x <= -50.0f) {
				worldTransform_.translation_.x = -50.0f;
			}
			if (worldTransform_.translation_.x >= 50.0f) {
				worldTransform_.translation_.x = 50.0f;
			}
		/*	ImGui::Begin("aaa");
			ImGui::DragFloat3("translation_", (float*)&worldTransform_.translation_, 0.01f, -100.0f, 100.0f);
			ImGui::End();*/
		}

		if (Input::GetInstance()->GetJoystickState(0, joyState))
		{
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
			}
		}

		if (isGoal) {
			kCharacterSpeed = 0;
		}

		// 座標移動(ベクトルの加算)
		worldTransform_.translation_.x += move.x;
		worldTransform_.translation_.y += move.y;

		// ワールドトランスフォームの更新
		worldTransform_.UpdateMatrix();


		GamePlayReset();

}

/// <summary>
/// 初期化
/// </ summary>
void Player::Draw(ViewProjection viewProjection){
	model_->Draw(viewProjection, worldTransform_);
}

void Player::OnCollisionX(){
}

void Player::OnCollisionY(){
}

void Player::OnCollisionGoal(){
	isGoal = true;
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::GamePlayReset()
{

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

		// 押した方向で移動ベクトルを変更(左右)
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (input_->PushKey(DIK_R))
		{
			isReset = true;
		}

		if (isReset == true)
		{
			Reset();
			isReset = false;
		}
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			
			isReset = true;
			
		}

		if (isReset == true)
		{
			Reset();
			isReset = false;
		}
	}
}

void Player::Reset()
{
	worldTransform_.translation_ = { -40, 2, 0 };
	isGoal = false;
	isStart = false;
}

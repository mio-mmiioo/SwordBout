#include "Player.h"
#include <assert.h>
#include "Camera.h"
#include "Stage.h"
#include "Bee.h"

namespace
{
	const float PLAYER_SPEED = 5.0f;
	VECTOR3 G = { 0, 9.8f, 0 };
}

Player::Player() : Player(VGet(0,0,0), 0.0f){}

Player::Player(const VECTOR3& pos, float rot)
{
	const std::string folder = "data/model/Character/Player/";
	// キャラモデルを読む
	hModel = MV1LoadModel((folder + "PC.mv1").c_str());
	assert(hModel > 0);
	// ルートノードをY軸回転する
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));

	animator = new Animator(hModel);
	assert(animator != nullptr);
	animator->AddFile(A_NEUTRAL,    folder + "Anim_Neutral.mv1",    true);
	animator->AddFile(A_RUN,        folder + "Anim_Run.mv1",        true);
	animator->AddFile(A_ATTACK1,    folder + "Anim_Attack1.mv1",    false);
	animator->AddFile(A_ATTACK2,    folder + "Anim_Attack2.mv1",    false);
	animator->AddFile(A_ATTACK3,    folder + "Anim_Attack3.mv1",    false);
	animator->AddFile(A_DAMAGE,     folder + "Anim_Damage.mv1",     false);
	animator->AddFile(A_BLOW_IN,    folder + "Anim_Blow_In.mv1",    false);
	animator->AddFile(A_BLOW_LOOP,  folder + "Anim_Blow_Loop.mv1",  true);
	animator->AddFile(A_BLOW_OUT,   folder + "Anim_Blow_Out.mv1",   false);
	animator->AddFile(A_DOWN,       folder + "Anim_Down_Loop.mv1",  true);
	animator->AddFile(A_GUARD_IN,   folder + "Anim_Guard_In.mv1",   false);
	animator->AddFile(A_GUARD_LOOP, folder + "Anim_Guard_Loop.mv1", true);
	animator->AddFile(A_GUARD_OUT,  folder + "Anim_Guard_Out.mv1",  false);
	animator->AddFile(A_JUMP_IN,    folder + "Anim_Jump_In.mv1",    false);
	animator->AddFile(A_JUMP_LOOP,  folder + "Anim_Jump_Loop.mv1",  true);
	animator->AddFile(A_JUMP_OUT,   folder + "Anim_Jump_Out.mv1",   false);
	animator->Play(A_NEUTRAL);

	transform.position = pos;
	transform.rotation.y = rot;

	camera = FindGameObject<Camera>();

	hSabel = MV1LoadModel("data/model/Character/Weapon/Sabel/Sabel.mv1");
	assert(hSabel > 0);

	state = S_FREE;
	//prevAttackKey = false;

	time = 0;
}

Player::~Player()
{
	if (hModel > 0)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}
}

void Player::Update()
{
	animator->Update();

	switch (state) {
	case S_FREE: UpdateFree(); break;
	case S_ATTACK1: UpdateAttack1(); break;
	case S_ATTACK2: UpdateAttack2(); break;
	case S_ATTACK3: UpdateAttack3(); break;
	case S_GUARD: UpdateGuard(); break;
	case S_JUMP: UpdateJump(); break;
	}

	camera->SetPlayerPosition(transform.position);
}

bool Player::HitAttackKey()
{
	bool current = (GetMouseInput() & MOUSE_INPUT_LEFT);

	if (current && !prevAttackKey)
	{
		prevAttackKey = current;
		return true;
	}

	prevAttackKey = current;
	return false;
}

void Player::AttackMain()
{
	int wp = MV1SearchFrame(hModel, "wp");
	MATRIX mWp = MV1GetFrameLocalWorldMatrix(hModel, wp);
	VECTOR3 nowSabelBottom = VECTOR3(0, 0, 0) * mWp;
	VECTOR3 nowSabelTop = VECTOR3(0, -200, 0) * mWp;

	//ここで当たり判定
	//std::list<Bee*> bees = FindGameObjects<Bee>();
	//for (Bee* b : bees)
	//{
	//	b->Attack(prevSabelBottom, prevSabelTop, nowSabelBottom, nowSabelTop);
	//}

	std::list<Actor*> enemies = FindGameObjects<Actor>();
	for (Actor* e : enemies)
	{
		e->Attack(prevSabelBottom, prevSabelTop, nowSabelBottom, nowSabelTop);
	}
	prevSabelTop = nowSabelTop;
	prevSabelBottom = nowSabelBottom;
}

void Player::UpdateFree()
{
	VECTOR3 move;

	//キー入力による移動
	if (CheckHitKey(KEY_INPUT_W))
	{
		move.z = 1;
		transform.rotation.y = camera->GetTransform().rotation.y;
		transform.position += move * PLAYER_SPEED * MGetRotY(transform.rotation.y);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		move.z = 1;
		transform.rotation.y = camera->GetTransform().rotation.y + 90 * DegToRad;
		transform.position += move * PLAYER_SPEED * MGetRotY(transform.rotation.y);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		move.z = 1;
		transform.rotation.y = camera->GetTransform().rotation.y - 90 * DegToRad;
		transform.position += move * PLAYER_SPEED * MGetRotY(transform.rotation.y);
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		move.z = 1;
		transform.rotation.y = camera->GetTransform().rotation.y + 180 * DegToRad;
		transform.position += move * PLAYER_SPEED * MGetRotY(transform.rotation.y);
	}

	if (CheckHitKey(KEY_INPUT_G))
	{
		animator->Play(AnimID::A_GUARD_IN);
		state = State::S_GUARD;
	}

	if (move.x + move.y + move.z == 0)
	{
		animator->Play(AnimID::A_NEUTRAL);
	}
	else
	{
		animator->Play(AnimID::A_RUN);
	}

	Stage* stage = FindGameObject<Stage>();

	VECTOR3 hit;
	VECTOR3& p = transform.position;
	if (stage->CollideLine(p + VECTOR3(0, 100, 0), p + VECTOR3(0, -100, 0), &hit))
	{
		p = hit;
		if (time != 0)
		time = 0;
	}
	else
	{
		//空中なら落下処理
		time += Time::DeltaTime();
		p -= G * time * time;
	}

	if (HitAttackKey())
	{
		animator->Play(A_ATTACK1);
		//animator->SetPlaySpeed(0.1f);//これを書くと当たっているかの判定をしやすい
		state = State::S_ATTACK1;
	}
}

void Player::UpdateAttack1()
{
	if (animator->IsFinish())
	{
		animator->Play(A_NEUTRAL);
		state = S_FREE;
	}
	AttackMain();
	if (animator->GetCurrentFrame() >= 8.5f)
	{
		if (HitAttackKey())
		{
			animator->Play(A_ATTACK2);
			state = State::S_ATTACK2;
		}
	}
}

void Player::UpdateAttack2()
{
	if (animator->IsFinish())
	{
		animator->Play(A_NEUTRAL);
		state = S_FREE;
	}
	if (animator->GetCurrentFrame() >= 9.5f)
	{
		if (HitAttackKey())
		{
			animator->Play(A_ATTACK3);
			state = State::S_ATTACK3;
		}
	}
}

void Player::UpdateAttack3()
{
	if (animator->IsFinish())
	{
		animator->Play(A_NEUTRAL);
		state = S_FREE;
	}
}

void Player::UpdateGuard()
{
	if (CheckHitKey(KEY_INPUT_G))
	{
		animator->Play(A_GUARD_LOOP);
	}
	else
	{
		animator->Play(A_GUARD_OUT);
		if (animator->IsFinish())
		{
			animator->Play(A_NEUTRAL);
			state = S_FREE;
		}
	}
}

void Player::UpdateJump()
{
}

void Player::Draw()
{
	Object3D::Draw();
	//Sabelの表示
	int wp = MV1SearchFrame(hModel, "wp");
	MATRIX mWp = MV1GetFrameLocalWorldMatrix(hModel, wp);
	MV1SetMatrix(hSabel, mWp);
	MV1DrawModel(hSabel);

	DrawLine3D(VECTOR3(0, 0, 0) * mWp, VECTOR3(0, -100, 0) * mWp, GetColor(255, 0, 0));
}
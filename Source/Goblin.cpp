#include "Goblin.h"
#include <assert.h>
#include "StateBase.h"
#include "Player.h"

Goblin::Goblin() : Goblin(VGet(0,0,0), 0.0f) {}

Goblin::Goblin(const VECTOR& pos, float rot)
{
	const std::string folder = "data/model/Character/Goblin/";
	hModel = MV1LoadModel((folder + "Goblin.mv1").c_str());
	assert(hModel > 0);

	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));

	animator = new Animator(hModel);
	assert(animator != nullptr);
	animator->AddFile(A_NEUTRAL,   folder + "Anim_Neutral.mv1",   true);
	animator->AddFile(A_WALK,      folder + "Anim_Walk.mv1",      true);
	animator->AddFile(A_RUN,       folder + "Anim_Run.mv1",       true);
	animator->AddFile(A_ATTACK1,   folder + "Anim_Attack1.mv1",   false);
	animator->AddFile(A_DAMAGE,    folder + "Anim_Damage.mv1",    false);
	animator->AddFile(A_BLOW_IN,   folder + "Anim_Blow_In.mv1",   false);
	animator->AddFile(A_BLOW_LOOP, folder + "Anim_Blow_Loop.mv1", true);
	animator->AddFile(A_BLOW_OUT,  folder + "Anim_Blow_Out.mv1",  false);
	animator->AddFile(A_DOWN,      folder + "Anim_Down_Loop.mv1",      true);
	animator->Play(A_NEUTRAL);

	transform.position = pos;
	transform.rotation.y = rot;

	coll = CapsuleCollider(VECTOR3(0, 150, 0), VECTOR3(0, 0, 0), 50);

#if false
	stateAction[State::S_WALK] = new GoblinWalk(this);
#endif
	state = S_WALK;
}

Goblin::~Goblin()
{
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}

	if (hModel > 0) {
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Goblin::Update()
{
#if false
	stateAction[state]->Update();
#endif
	animator->Update();

	switch (state) {
	case State::S_WALK:
		UpdateWalk();
		break;
	case State::S_RUN:
		UpdateRun();
		break;
	default:
		break;
	}

}

void Goblin::Attack(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop)
{
	if (IsHit(prevBtm, prevTop, nowBtm, nowTop))
	{
		animator->Play(A_DAMAGE);
		damaged = true;
	}
}

//v1・v2 = |v1| * |v2| * cos

void Goblin::UpdateWalk()
{
	animator->Play(A_WALK);

	//プレイヤーが視野に入ったら、S_RUNにする
	Player* player = FindGameObject<Player>();
	VECTOR3 playerPos = player->GetTransform().position;

	//距離　10ｍ→1000

	VECTOR3 toPlayer = playerPos - transform.position;

	if (toPlayer.Size() < 1000)
	{
		//視野に入ったら走る
		VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(transform.rotation.y);
		float ip = VDot(front, toPlayer.Normalize()); //内積　cosθ
		//if(acos(ip) < 30*DegToRad)
		static const float limit = cosf(30 * DegToRad);
		if (ip >= limit)
		{
			state = S_RUN;

		}
	}
}

void Goblin::UpdateRun()
{
	animator->Play(A_RUN);
	Player* player = FindGameObject<Player>();
	VECTOR3 playerPos = player->GetTransform().position;
	VECTOR3 toPlayer = playerPos - transform.position;

	VECTOR3 v = VECTOR3(0, 0, 1) * MGetRotY(toPlayer.y);

	transform.position += toPlayer.Normalize() * 3.5;

	if (toPlayer.Size() > 1000)
	{
		state = S_WALK;
	}
}

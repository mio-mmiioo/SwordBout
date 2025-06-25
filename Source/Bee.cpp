#include "Bee.h"
#include <assert.h>

Bee::Bee() : Bee(VGet(0, 0, 0), 0.0f){}

Bee::Bee(const VECTOR& pos, float rot)
{
	const std::string folder = "data/model/Character/Bee/";
	hModel = MV1LoadModel((folder + "Bee.mv1").c_str());
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));

	animator = new Animator(hModel);
	assert(animator != nullptr);
	animator->AddFile(A_NEUTRAL, folder + "Anim_Neutral.mv1", true);
	animator->AddFile(A_WALK,    folder + "Anim_Walk.mv1",    true);
	animator->AddFile(A_RUN,     folder + "Anim_Run.mv1",     true);
	animator->AddFile(A_ATTACK1, folder + "Anim_Attack1.mv1", false);
	animator->AddFile(A_ATTACK2, folder + "Anim_Attack2.mv1", false);
	animator->AddFile(A_DAMAGE,  folder + "Anim_Damage.mv1",  false);
	animator->AddFile(A_DOWN,    folder + "Anim_Down.mv1",    true);
	animator->Play(A_NEUTRAL);

	transform.position = pos;
	transform.rotation.y = rot;
}

Bee::~Bee()
{
	if (animator!=nullptr) {
		delete animator;
		animator = nullptr;
	}

	if (hModel > 0) {
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Bee::Update()
{
	animator->Update();

	if (animator->GetCurrentID() == A_DAMAGE)
	{
		if (animator->IsFinish())
		{
			animator->Play(A_NEUTRAL);
		}
	}
}

//float Segment_Triangle_MinLength(
//	VECTOR SegmentPos1, 
//	VECTOR SegmentPos2, 
//	VECTOR TrianglePos1, 
//	VECTOR TrianglePos2, 
//	VECTOR TrianglePos3);
//@‚±‚Ì‹——£‚ª50ˆÈ“à‚É‚È‚Á‚½‚ç“–‚½‚Á‚Ä‚¢‚é

void Bee::Attack(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop)
{
	VECTOR3 SegmentPos1 = transform.position + VECTOR3(0, 190, 0);
	VECTOR3 SegmentPos2 = transform.position + VECTOR3(0, 90, 0);
	if (IsHit(SegmentPos1, SegmentPos2, 50.0f, prevBtm, prevTop, nowBtm, nowTop))
	{
		animator->Play(A_DAMAGE);
	}
}

bool Bee::IsHit(VECTOR3 segmentPos1, VECTOR3 segmentPos2, float distance, VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop)
{
	float check1 = Segment_Triangle_MinLength(segmentPos1, segmentPos2, prevBtm, prevTop, nowBtm);
	float check2 = Segment_Triangle_MinLength(segmentPos1, segmentPos2, prevTop, nowBtm, nowTop);

	if (check1 <= distance || check2 <= distance)
	{
		return true;
	}

	return false;
}

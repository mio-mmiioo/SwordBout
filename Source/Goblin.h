#pragma once
#include "Actor.h"
#include "Animator.h"
#include "StateBase.h"

class GoblinWalk : public StateBase
{
public:
	GoblinWalk(Actor* parent) : StateBase(parent) {}
	~GoblinWalk() {}
	void Update() override;
	void Suspend() override;
	void Resume() override;
};

class Goblin : public Actor {
public:
	friend class GoblinWalk;//friend class 自分のクラスにアクセスしていいよって許可

	Goblin();
	Goblin(const VECTOR& pos, float rot);
	~Goblin();
	void Update() override;
	void Attack(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop) override;
private:
	enum AnimID {
		A_NEUTRAL = 0,
		A_WALK,
		A_RUN,
		A_ATTACK1,
		A_DAMAGE,
		A_BLOW_IN,
		A_BLOW_LOOP,
		A_BLOW_OUT,
		A_DOWN,
	};
	Animator* animator;

	enum State {
		S_WALK = 0,
		S_RUN,
		S_ATTACK,
		S_WAIT,
		S_BACK,
		S_DAMAGE,
		S_BLOW,
		S_MAX
	};
	State state;
	void UpdateWalk();
	void UpdateRun();
	void UpdateBack();

	VECTOR3 basePosition;//自分が生成された場所

	void MoveSet(VECTOR3 toPosition, float angSpeed, float moveSpeed);//どこかへ向かうときの関数 向かう場所の座標を入れる

#if false //クラスバージョン
	StateBase* stateAction[State::S_MAX];
#endif
};
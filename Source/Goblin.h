#pragma once
#include "Actor.h"
#include "Animator.h"

class Goblin : public Actor {
public:
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
};
#pragma once
#include "Actor.h"
#include "Animator.h"

class Bee : public Actor {
public:
	Bee();
	Bee(const VECTOR& pos, float rot);
	~Bee();
	void Update() override;

	void Attack(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop) override;

private:
	enum AnimID {
		A_NEUTRAL = 0,
		A_WALK,
		A_RUN,
		A_ATTACK1,
		A_ATTACK2,
		A_DAMAGE,
		A_DOWN,
	};
	Animator* animator;
};
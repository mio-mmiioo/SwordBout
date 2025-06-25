#pragma once
#include "Object3D.h"

struct CapsuleCollider {
	VECTOR3 pos1;
	VECTOR3 pos2;
	float radius;
	CapsuleCollider(VECTOR3 p1, VECTOR3 p2, float rad) : pos1(p1), pos2(p2), radius(rad){}
	CapsuleCollider() : pos1(VECTOR3(0, 0, 0)), pos2(VECTOR3(0, 1, 0)), radius(0) {}
};

class Actor : public Object3D
{
public:
	Actor() { damaged = false; }
	virtual ~Actor() {}
	virtual void Update() override {}
	virtual void Draw() override;

	/// <summary>
	/// ãﬂê⁄Ç≈çUåÇÇ≥ÇÍÇΩ
	/// </summary>
	/// <param name="prevBtm"></param>
	/// <param name="prevTop"></param>
	/// <param name="nowBtm"></param>
	/// <param name="nowTop"></param>
	virtual void Attack(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop);
protected:
	bool IsHit(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop);
	CapsuleCollider coll;
	bool damaged;
};


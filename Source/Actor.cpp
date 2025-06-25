#include "Actor.h"

void Actor::Draw()
{
	Object3D::Draw();
	unsigned int color = GetColor(255, 0, 0);

	if (damaged) {
		color = GetColor(255, 255, 255);
	}
	DrawCapsule3D(transform.position + coll.pos1, transform.position + coll.pos2, coll.radius, 20, color, color, FALSE);
}

void Actor::Attack(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop)
{
	if (IsHit(prevBtm, prevTop, nowBtm, nowTop))
	{
		damaged = true;
	}
}

bool Actor::IsHit(VECTOR3 prevBtm, VECTOR3 prevTop, VECTOR3 nowBtm, VECTOR3 nowTop)
{
	VECTOR3 segmentPos1 = transform.position + coll.pos1;
	VECTOR3 segmentPos2 = transform.position + coll.pos2;
	
	float distance = coll.radius;
	if (Segment_Triangle_MinLength(segmentPos1, segmentPos2, prevBtm, prevTop, nowTop) < distance)
		return true;

	if (Segment_Triangle_MinLength(segmentPos1, segmentPos2, prevBtm, nowTop, nowBtm) < distance)
		return true;

	return false;
}

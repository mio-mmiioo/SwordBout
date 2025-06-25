#pragma once

class Actor;

class StateBase
{
public:
	StateBase(Actor* base) { parent = base; }
	virtual ~StateBase();
	virtual void Update();//XVˆ—
	virtual void Suspend();//’â~ˆ—
	virtual void Resume();//ÄŠJˆ—
protected:
	Actor* parent;
};


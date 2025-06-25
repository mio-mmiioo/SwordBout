#pragma once

class Actor;

class StateBase
{
public:
	StateBase(Actor* base) { parent = base; }
	virtual ~StateBase();
	virtual void Update();//�X�V����
	virtual void Suspend();//��~����
	virtual void Resume();//�ĊJ����
protected:
	Actor* parent;
};


#pragma once
#include "../Library/gameObject.h"
#include "Transform.h"

/// <summary>
/// 3D�@�\���������AGameObject�̊��N���X
/// </summary>
class Object3D : public GameObject {
public:
	Object3D() : hModel(-1), parent(nullptr) {}
	virtual ~Object3D();
	void Draw() override;

	/// <summary>
	/// Transform�f�[�^���Q�ƂŎ擾����
	/// </summary>
	/// <returns>Transform�f�[�^</returns>
	const Transform& GetTransform() { return transform; }

	/// <summary>
	/// �e�I�u�W�F�N�g��ݒ肷��
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	void SetParent(Object3D* _parent) { parent = _parent; }
protected:
	int hModel;
	Transform transform;
	Object3D* parent;
};
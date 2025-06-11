#pragma once
#include "../Library/myDxLib.h"

/// <summary>
/// 3D�̕ό`�p�����[�^�[
/// </summary>
struct Transform {
public:
	VECTOR3 position; // �ʒu
	VECTOR3 rotation; // �����Ƃ̉�]
	VECTOR3 scale; // �����Ƃ̊g�k

	Transform() {
		position = VZero;
		rotation = VZero;
		scale = VECTOR3(1.0f, 1.0f, 1.0f);
		rotMatrix = MGetIdent();
		localMatrix = MGetIdent();
	}
	~Transform() {}

	/// <summary>
	/// �ʒu�E��]�E�g�k���g�����s������߂�
	/// </summary>
	/// <returns>�v�Z����</returns>
	const MATRIX& MakeLocalMatrix();

	/// <summary>
	/// �ʒu�E��]�E�g�k���g�����s���Ԃ�
	/// �����ł͌v�Z���Ă��Ȃ��āAMakeLocalMatrix()�Ōv�Z�������̒l���g��
	/// </summary>
	/// <returns>�ό`�s��</returns>
	const MATRIX& GetLocalMatrix() const { return localMatrix; }

	/// <summary>
	/// ��]�Ɗg�k���g�����s���Ԃ�
	/// �ʒu�͊܂܂Ȃ��̂ŁA��Ƀx�N�g���̌v�Z�Ŏg��
	/// �����ł͌v�Z���Ă��Ȃ��āAMakeLocalMatrix()���Ōv�Z�������̒l���g��
	/// </summary>
	/// <returns>��]�s��</returns>
	const MATRIX& GetRotationMatrix() const { return rotMatrix; }
private:
	MATRIX rotMatrix;
	MATRIX localMatrix;
};
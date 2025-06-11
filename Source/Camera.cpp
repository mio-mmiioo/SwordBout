#include "Camera.h"
#include "../ImGui/imgui.h"

namespace {
	static float distance = 500.0f; // �L��������̋���
	static float lookHeight = 200.0f; // �����_�̍���
	const float CAMERA_SPEED = 0.005f; // �J�����̉�]���x
};

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
	transform.rotation.y = 20.0f * DegToRad;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	int mouseX, mouseY;

	GetMousePoint(&mouseX, &mouseY);
	int moveX = mouseX - prevX;
	int moveY = mouseY - prevY;

	VECTOR3& rot = transform.rotation;//�Q�ƂŏE���Ă����Ώ����������ł���

	rot.y += CAMERA_SPEED * moveX;
	rot.x -= CAMERA_SPEED * moveY;

	if (rot.x >= 80 * DegToRad)
	{
		rot.x = 80 * DegToRad;
	}
	if (rot.x < -25 * DegToRad)
	{
		rot.x = -25 * DegToRad;
	}

	//�L�[�ɂ��J������]
	{
		//if (CheckHitKey(KEY_INPUT_RIGHT))
		//{
		//	rot.y += CAMERA_SPEED * DegToRad;
		//}
		//if (CheckHitKey(KEY_INPUT_LEFT))
		//{
		//	rot.y -= CAMERA_SPEED * DegToRad;
		//	//if (rot.y < 0)
		//	//{
		//	//	rot.y += 2.0f;
		//	//}
		//}
		//if (CheckHitKey(KEY_INPUT_UP))
		//{
		//	rot.x += CAMERA_SPEED * DegToRad;
		//	if (rot.x > 2.0f)
		//	{
		//		rot.x -= 2.0f;
		//	}
		//	if (rot.x >= 80 * DegToRad)
		//	{
		//		rot.x = 80 * DegToRad;
		//	}
		//}
		//if (CheckHitKey(KEY_INPUT_DOWN))
		//{
		//	rot.x -= 3.0f * DegToRad;

		//	if (rot.x < -25 * DegToRad)
		//	{
		//		rot.x = -25 * DegToRad;
		//	}
		//}

	}

	ImGui::Begin("Camera");
	ImGui::InputFloat("RotX", &rot.x);
	ImGui::InputFloat("RotY", &rot.y);
	ImGui::InputFloat("RotZ", &rot.z);
	ImGui::End();

	VECTOR3 playerHeadPos = VECTOR3(0, 150.0f, 0);
	VECTOR3 camPos = VECTOR3(0, 0, -500.0f) * MGetRotX(rot.x) * MGetRotY(rot.y);
	//VECTOR3 camPos = VECTOR3(0, 300.0f, -400.0f) * MGetRotX(rot.x) * MGetRotY(rot.y);

	SetCameraPositionAndTarget_UpVecY(
			lookPosition + camPos + playerHeadPos,
			lookPosition + VECTOR3(0, 200.0f, 0));

	prevX = mouseX;
	prevY = mouseY;
}

void Camera::Draw()
{
}

void Camera::SetPlayerPosition(const VECTOR& pos)
{
	lookPosition = pos;
}

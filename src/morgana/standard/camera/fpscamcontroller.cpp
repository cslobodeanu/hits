#include "fpscamcontroller.h"

using namespace MorganaEngine::Standard::Cameras;

__implement_morgana_script1(FpsCamController)

FpsCamController::FpsCamController()
{
	camera = NULL;
	moveSpeed = 20.0f;
	rotSpeed = 1.5f;

	DoAddProperties();
}

FpsCamController::~FpsCamController()
{
}

void FpsCamController::Update()
{
	super::Update();

	if (Widget::capture != Desktop::main && Widget::capture != NULL) return;

	if (camera == NULL)
		camera = Scene::Cameras::Camera::main;

	if (camera == NULL) return;

	float multiplier = 1.0f;
	if (Input::GetKeyDown(Keys::Shift) || Input::GetKeyDown(Keys::LeftShift) || Input::GetKeyDown(Keys::RightShift))
	{
		multiplier = 2.0f;
	}

	Vector3 p = camera->transform->position;
	if (Input::GetKeyDown(Keys::W))
	{
		p += camera->transform->rotation->Forward() * moveSpeed * Time::deltaTime * multiplier;
	}

	if (Input::GetKeyDown(Keys::S))
	{
		p -= camera->transform->rotation->Forward() * moveSpeed * Time::deltaTime * multiplier;
	}

	if (Input::GetKeyDown(Keys::A))
	{
		p -= camera->transform->rotation->RightDir() * moveSpeed * Time::deltaTime * multiplier;
	}

	if (Input::GetKeyDown(Keys::D))
	{
		p += camera->transform->rotation->RightDir() * moveSpeed * Time::deltaTime * multiplier;
	}

	camera->transform->position = p;

	if (Input::GetMouseButtonPressed(MouseButtons::MouseLeft))
	{
		prevMousePos = Input::GetMousePos();
		prevCameraPos = camera->transform->position;
	}

	if (Input::GetMouseButtonDown(MouseButtons::MouseLeft))
	{
		if (Input::GetKeyDown(Keys::Space))
		{
			p = prevCameraPos;

			p -= (Input::GetMousePos().x - prevMousePos.x) * moveSpeed * camera->transform->rotation->RightDir() * 1.5f * multiplier;
			p += (Input::GetMousePos().y - prevMousePos.y) * moveSpeed * camera->transform->rotation->UpDir() * 1.5f * multiplier;

			camera->transform->position = p;
		}
		else
		{
			Vector3 angles = camera->transform->rotation->ToEuler();
			angles.x -= (Input::GetMousePos().y - prevMousePos.y) * rotSpeed * multiplier;
			angles.y -= (Input::GetMousePos().x - prevMousePos.x) * rotSpeed * multiplier;
			angles.x = Mathf::Clamp(angles.x, -89.9f, 89.9f);
			camera->transform->rotation = Quaternion::FromEuler(angles);
		}
	}

	if (Input::GetMouseWheel() != 0)
	{
		p = camera->transform->position;
		p += camera->transform->rotation->Forward() * moveSpeed * (float)Input::GetMouseWheel() * 0.1f * multiplier;
		camera->transform->position = p;
	}
}

void FpsCamController::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(moveSpeed);
}
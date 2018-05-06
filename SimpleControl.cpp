#include "SimpleControl.h"

void SimpleControl::RegisterObject(Context * context)
{
	context->RegisterFactory<SimpleControl>();
}

SimpleControl::SimpleControl(Context * context) : LogicComponent(context)
{
	SetUpdateEventMask(USE_FIXEDUPDATE | USE_UPDATE);
}

void SimpleControl::Start()
{

}

void SimpleControl::Update(float timeStep)
{
	ViewRotate(timeStep);
	ViewMove(timeStep);
}

void SimpleControl::FixedUpdate(float timeStep)
{

}

void SimpleControl::OnNodeSet(Node * node)
{
	pNode = node;
}

void SimpleControl::ViewRotate(float timeStep)
{
	static Input* input = GetSubsystem<Input>();

	const float MOUSE_SENSITIVITY = 0.1f;
	IntVector2 mouseMove = input->GetMouseMove();

	//Node* node = GetNode();
	
	if (pNode)
	{
		Quaternion q = pNode->GetWorldRotation();
		float yaw = q.YawAngle();
		float pitch = q.PitchAngle();

		yaw += MOUSE_SENSITIVITY * mouseMove.x_;
		pitch += MOUSE_SENSITIVITY * mouseMove.y_;
		pitch = Clamp(pitch, -90.0f, 90.0f);


		pNode->SetWorldRotation(Quaternion(pitch, yaw, 0.0f));
	}
}

void SimpleControl::ViewMove(float timeStep)
{
	//static Node* node = GetNode();

	if (pNode)
	{
		const float MOVE_SPEED = 5.0f;

		static Input* input = GetSubsystem<Input>();

		Quaternion rotation = pNode->GetWorldRotation();

		if (input->GetKeyDown('W'))
			pNode->Translate((rotation * Vector3::FORWARD) * MOVE_SPEED * timeStep, TS_WORLD);

		if (input->GetKeyDown('S'))
			pNode->Translate((rotation * Vector3::BACK) * MOVE_SPEED * timeStep, TS_WORLD);

		if (input->GetKeyDown('A'))
			pNode->Translate((rotation * Vector3::LEFT) * MOVE_SPEED * timeStep, TS_WORLD);

		if (input->GetKeyDown('D'))
			pNode->Translate((rotation * Vector3::RIGHT) * MOVE_SPEED * timeStep, TS_WORLD);

		if (input->GetKeyDown(KEY_SPACE))
			pNode->Translate(Vector3::UP * MOVE_SPEED * timeStep, TS_WORLD);

		if (input->GetKeyDown('Z'))
			pNode->Translate(Vector3::DOWN * MOVE_SPEED * timeStep, TS_WORLD);



	}
}

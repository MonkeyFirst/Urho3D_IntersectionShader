#pragma once
#include <Urho3D/Urho3DAll.h>

class SimpleControl : public LogicComponent
{
	URHO3D_OBJECT(SimpleControl, LogicComponent);
public:
	static void RegisterObject(Context* context);
public:
	SimpleControl(Context* context);

	void Start();
	virtual void Update(float timeStep);
	virtual void FixedUpdate(float timeStep);
	virtual ~SimpleControl() {};
protected:
	void OnNodeSet(Node* node) override;
private:
	void ViewRotate(float timeStep);
	void ViewMove(float timeStep);

	Node* pNode = nullptr;

};


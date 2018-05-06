#pragma once
#include <Urho3D/Urho3DAll.h>
#include <vector>

class CameraManager : public Object
{
	URHO3D_OBJECT(CameraManager, Object);
public:

	struct  SCameraView
	{
		SCameraView() : node(nullptr), camera(nullptr), viewport(nullptr) {}
		Node* node;
		Camera* camera;
		Viewport* viewport;
	};

public:
	CameraManager(Context* context);
	virtual ~CameraManager();

	void BindScene(Scene* pScene) { pBindScene = pScene; };
	bool FindAllSceneCameras(Scene * scene_);
	bool FindAllSceneCameras();
	bool ActivateView(unsigned int index = 0);
	bool ActivateView(String name);

	SCameraView& GetView(unsigned int index) { return m_Cameras[index]; };
	SCameraView& GetCurrentView() { return m_Cameras[m_currentViewIndex]; };


	unsigned int CreateView(String name);
	void RemoveView(String name);
	void RemoveView(unsigned int index);

 public:
	Scene* pBindScene;
	PODVector<SCameraView> m_Cameras;
	RenderPath* m_pRenderPath = nullptr;
	unsigned m_currentViewIndex = 0;
};
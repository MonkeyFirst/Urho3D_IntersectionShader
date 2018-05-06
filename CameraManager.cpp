#include <Urho3D\Urho3DAll.h>

#include "CameraManager.h"

CameraManager::CameraManager(Context * context) : Object(context)
{

}

CameraManager::~CameraManager()
{
}

bool CameraManager::FindAllSceneCameras(Scene * scene_)
{
	if (!scene_) return false;

	PODVector<Node*> nodes;
	scene_->GetChildrenWithComponent<Camera>(nodes, true);
	
	// Reset
	m_Cameras.Clear();
	m_currentViewIndex = 0;
	m_pRenderPath = nullptr;

	// Collect
	if (!nodes.Empty()) 
	{
		for (unsigned int i = 0; i < nodes.Size(); i++)
		{
			SCameraView cv;
			cv.node = nodes[i];
			cv.camera = nodes[i]->GetComponent<Camera>();
			m_Cameras.Push(cv);
		}
	}

	return m_Cameras.Size() > 0;
}

bool CameraManager::FindAllSceneCameras()
{
	return FindAllSceneCameras(pBindScene);
}

bool CameraManager::ActivateView(unsigned int index)
{
	bool ret = false;

	if (m_Cameras.Empty())
		return false;

	if (m_Cameras.Size() >= index ) 
	{
		SCameraView& cv = m_Cameras[index];
		m_currentViewIndex = index;
		if (cv.camera != nullptr && cv.node != nullptr)
		{
			if (!cv.viewport)
				cv.viewport = new Viewport(context_, cv.node->GetScene(), cv.camera);

			if (cv.viewport)
			{
				ret = true;
				GetSubsystem<Renderer>()->SetViewport(0, cv.viewport);
				m_pRenderPath = cv.viewport->GetRenderPath();
			}
		}
	}

	return ret;
}

bool CameraManager::ActivateView(String name)
{
	for (unsigned int i = 0; i < m_Cameras.Size(); i++)
	{
		if (m_Cameras[i].node->GetName() == name)
			return ActivateView(i);
	}
	return false;
}

unsigned int CameraManager::CreateView(String name)
{
	SCameraView cv;
	cv.node = pBindScene->CreateChild(name, CreateMode::LOCAL);
	cv.camera = cv.node->CreateComponent<Camera>();
	cv.viewport = new Viewport(context_, cv.node->GetScene(), cv.camera);
	m_Cameras.Push(cv);
	return m_Cameras.Size();
}

void CameraManager::RemoveView(String name)
{
	for (unsigned int i = 0; i < m_Cameras.Size(); i++)
	{
		if (m_Cameras[i].node->GetName() == name)
			return RemoveView(i);
	}
}

void CameraManager::RemoveView(unsigned int index)
{
	if (index <= m_Cameras.Size())
	{
		
		Viewport* cvp = GetSubsystem<Renderer>()->GetViewport(0);
		SCameraView cv = m_Cameras[index];

		if (cv.viewport == cvp) 
		{
			GetSubsystem<Renderer>()->SetViewport(0, nullptr);
			cv.viewport->ReleaseRef();
			cv.viewport = nullptr;
		}

		cv.node->RemoveComponent<Camera>();
		cv.camera = nullptr;
		pBindScene->RemoveChild(cv.node);
		
		m_Cameras.Erase(m_Cameras.Begin() + index);
	}
}

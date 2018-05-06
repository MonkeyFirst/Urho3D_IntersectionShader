#include <Urho3D/Urho3DAll.h>
#include "CameraManager.h"
#include "SimpleControl.h"

using namespace Urho3D;
class MyApp : public Application
{

public:
	Scene* scene_;
	CameraManager* pCameraManager;

public:
    MyApp(Context* context) :
        Application(context)
    {
		SimpleControl::RegisterObject(context_);
    }
    virtual void Setup()
    {
		context_->RegisterSubsystem(new CameraManager(context_));

        // Called before engine initialization. engineParameters_ member variable can be modified here
		engineParameters_[EP_WINDOW_WIDTH] = 1280;
		engineParameters_[EP_WINDOW_HEIGHT] = 720;
		engineParameters_[EP_FULL_SCREEN] = false;
		engineParameters_[EP_LOG_NAME] = GetSubsystem<FileSystem>()->GetProgramDir() + GetTypeName() + ".log";
		engineParameters_[EP_RENDER_PATH] = "CoreData/RenderPaths/Deferred.xml";
    }
    virtual void Start()
    {
		pCameraManager = GetSubsystem<CameraManager>();

		scene_ = new Scene(context_);
		String path = GetSubsystem<FileSystem>()->GetProgramDir();
		File sceneFile(context_, path + "Data/Scenes/test1.xml", FILE_READ);
		scene_->LoadXML(sceneFile);

		pCameraManager->BindScene(scene_);
		pCameraManager->FindAllSceneCameras();
		pCameraManager->ActivateView(0);
		pCameraManager->GetCurrentView().node->CreateComponent<SimpleControl>();

        // Called after engine initialization. Setup application & subscribe to events here
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, HandleKeyDown));
    }
    virtual void Stop()
    {
        // Perform optional cleanup after main loop has terminated
    }
    void HandleKeyDown(StringHash eventType, VariantMap& eventData)
    {
        using namespace KeyDown;
        // Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
        int key = eventData[P_KEY].GetInt();
        if (key == KEY_ESCAPE)
            engine_->Exit();
    }
};
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)

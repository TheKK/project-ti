#ifndef CAMERA_LOOK_AT_EVENT_SCRIPT_H
#define CAMERA_LOOK_AT_EVENT_SCRIPT_H

#include <SDL2/SDL.h>

#include "eventScript.h"

class Camera;
class Player;
class MainGame_GameScene;

class CameraLookAt_eventScript : public EventScript
{
public:
	CameraLookAt_eventScript(MainGame_GameScene& gameScene, Camera& camera,
				 const Player* player);

	virtual void update() override;
private:
	MainGame_GameScene& gameScene_;
	Camera& camera_;
	const Player* player_;
};

#endif /* CAMERA_LOOK_AT_EVENT_SCRIPT_H */

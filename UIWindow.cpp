#include "UIWindow.h"

#include "InputManager.h"
#include "SceneManager.h"

UIWindow::UIWindow(std::string name, SceneManager* manager)
{
	mName = name;
	mCurrentSceneManager = manager;

	lastMouse = glm::vec2(-10.0);
}


UIWindow::~UIWindow()
{
}

void UIWindow::update()
{
	if (mSelected)
	{
		mColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	}

	if (mSelected && movable_)
	{
		//First time
		if (lastMouse.x == -10)
		{
			lastMouse = mCurrentSceneManager->getMousePosition_NDC();
		}

		if (InputManager::getSingletonPtr()->isMouseButtonDown(SDL_BUTTON_LEFT) && rayTestToObject(mCurrentSceneManager->getMousePosition_NDC()))
		{
			mPosition += glm::vec3((mCurrentSceneManager->getMousePosition_NDC() - lastMouse), 0.0);
			lastMouse = mCurrentSceneManager->getMousePosition_NDC();
		}

		if(!InputManager::getSingletonPtr()->isMouseButtonDown(SDL_BUTTON_LEFT))
		{
			lastMouse = glm::vec2(-10.0);
		}
	}
}
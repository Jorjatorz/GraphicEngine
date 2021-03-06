#include "InputManager.h"

#include "ResourceManager.h"
#include "Root.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "UIManager.h"
#include "UIDisplayer.h"

InputManager::InputManager(void)
{
	//initialize array to all false
	for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		mKeyPressedArray[i] = false;
	}

	for(int i = 0; i < 3; ++i)
	{
		mMousePressedArray[i] = false;
	}

	mSDLWindow = NULL;
}


InputManager::~InputManager(void)
{
}


void InputManager::getFrameInput(bool &running)
{
	while(SDL_PollEvent(&mEvent))
	{
		if(mEvent.type == SDL_QUIT)
		{
			running = false;
			break;
		}

		if(mEvent.type == SDL_WINDOWEVENT)
		{
			if(mEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
			{
				std::cout << "Focus Gained" << std::endl;
				ResourceManager* mR = ResourceManager::getSingletonPtr();
				mR->checkForModifications();
			}

		}
		
		if(mEvent.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&mouseX, &mouseY);
			Camera* mCam = Root::getSingletonPtr()->getCurrentSceneManager()->getCurrentCamera();
			//Check that a camera exists
			if (mCam != NULL)
			{
				mCam->handleMouseMove(this, mouseX, mouseY);
			}

			//std::cout << mouseX << " " << mouseY << std::endl;

			UIDisplayer* displayer = Root::getSingletonPtr()->mUIManager->getCurrentDisplayer();
			if (displayer != NULL)
			{
				displayer->setMouseMove(glm::vec2(mouseX, mouseY));
			}
		}

		if(mEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			mMousePressedArray[mEvent.button.button - 1] = true; //-1 to pass from sdl encoding to array acces encoding

			Root::getSingletonPtr()->mUIManager->getCurrentDisplayer()->setMouseButtonDown();

			if ((Root::getSingletonPtr()->getCurrentSceneManager()->isEditorModeOn()) && (Root::getSingletonPtr()->mUIManager->getCurrentDisplayer()->getFocusWindow() == NULL))
			{
				if (mEvent.button.button == SDL_BUTTON_LEFT)
				{
					Root::getSingletonPtr()->getCurrentSceneManager()->injectMouseDown_WorldEditor();
				}
			}
		}

		if(mEvent.type == SDL_MOUSEBUTTONUP)
		{
			mMousePressedArray[mEvent.button.button - 1] = false;

			Root::getSingletonPtr()->mUIManager->getCurrentDisplayer()->setMouseButtonUp();
		}

		if(mEvent.type == SDL_KEYDOWN)
			mKeyPressedArray[mEvent.key.keysym.scancode] = true;

		if(mEvent.type == SDL_KEYUP)
			mKeyPressedArray[mEvent.key.keysym.scancode] = false;
	}
}

bool InputManager::isKeyDown(short int scanCode)
{
	return mKeyPressedArray[scanCode];
}

bool InputManager::isKeyUp(short int scanCode)
{
	//return the array negated because if the key is not pressed (false) then its up (so !false = true)
	return !mKeyPressedArray[scanCode];
}

bool InputManager::isMouseButtonDown(short int buttonCode)
{
	return mMousePressedArray[buttonCode - 1]; //transform from 1, 2, 3 sdl numbers into 0, 1, 2 array acces numbers
}
bool InputManager::isMouseButtonUp(short int buttonCode)
{
	return !mMousePressedArray[buttonCode - 1];
}

void InputManager::setMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(mSDLWindow, x, y);
}

void InputManager::getMousePosition(glm::vec2& mousePos)
{
	mousePos = glm::vec2(mouseX, mouseY);
}

void InputManager::getMousePosition(int &x, int &y)
{
	x = mouseX;
	y = mouseY;
}

glm::vec2 InputManager::getMousePosition_NDC()
{
	int windowH, windowW;
	windowW = Root::getSingletonPtr()->mRenderer->getCurrentWindow()->getWidth();
	windowH = Root::getSingletonPtr()->mRenderer->getCurrentWindow()->getHeight();
	real x = (2.0f * mouseX) / windowW - 1.0f;
	real y = 1.0f - (2.0f * mouseY) / windowH;

	return glm::vec2(x, y);
}
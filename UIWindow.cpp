#include "UIWindow.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "UIButton.h"
#include "UILabel.h"

UIWindow::UIWindow(std::string name, SceneManager* manager)
{
	mName = name;
	mCurrentSceneManager = manager;

	mTextPosition = glm::vec2(0.0, mSize.y / 2.0f - 0.1);

	lastMouse = glm::vec2(-10.0);
}


UIWindow::~UIWindow()
{
	tWindowObjectsMap::iterator it;
	for (it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
	{
		delete it->second;
	}
	mObjectsMap.clear();
}

void UIWindow::update()
{
	if (mSelected)
	{
		mColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	}

	if (mouseDown_ && movable_)
	{
		//First time
		if (lastMouse.x == -10)
		{
			lastMouse = mCurrentSceneManager->getMousePosition_NDC();
		}

		if (InputManager::getSingletonPtr()->isMouseButtonDown(SDL_BUTTON_LEFT) && rayTestToObject(mCurrentSceneManager->getMousePosition_NDC()))
		{

			//If we are on the lower right corner
			if ((mCurrentSceneManager->getMousePosition_NDC().x > (mSize.x/2 - 0.1) + mPosition.x) && (mCurrentSceneManager->getMousePosition_NDC().y < (-mSize.y/2 + 0.1) + mPosition.y))
			{
				mSize.x += (mCurrentSceneManager->getMousePosition_NDC() - lastMouse).x * 2.0f;
				mSize.y -= (mCurrentSceneManager->getMousePosition_NDC() - lastMouse).y * 2.0f;

				mTextPosition.x = -mSize.x * mParentSize.x / 50.0f;
				mTextPosition.y = (mSize.y - 0.15)  / 2.0f ;

				lastMouse = mCurrentSceneManager->getMousePosition_NDC();
			}
			else
			{
				mPosition += mCurrentSceneManager->getMousePosition_NDC() - lastMouse;
				lastMouse = mCurrentSceneManager->getMousePosition_NDC();
			}
		}
	}
}

UIButton* UIWindow::createButton(std::string name)
{
	tWindowObjectsMap::iterator it = mObjectsMap.find(name);
	if (it != mObjectsMap.end())
	{
		return static_cast<UIButton*>(it->second);
	}

	UIButton* newButton = new UIButton(name, this, mCurrentSceneManager);

	mObjectsMap.insert(std::pair<std::string, UIButton*>(name, newButton));
	return newButton;
}

void UIWindow::deleteObject(std::string name)
{
	tWindowObjectsMap::iterator it = mObjectsMap.find(name);
	if (it != mObjectsMap.end())
	{
		delete it->second;
	}
}

UILabel* UIWindow::createLabel(std::string name)
{
	tWindowObjectsMap::iterator it = mObjectsMap.find(name);
	if (it != mObjectsMap.end())
	{
		return static_cast<UILabel*>(it->second);
	}

	UILabel* newLabel = new UILabel(name, this, mCurrentSceneManager);

	mObjectsMap.insert(std::pair<std::string, UILabel*>(name, newLabel));
	return newLabel;
}


bool UIWindow::rayTestToChilds(glm::vec2 mouseCoords)
{
	tWindowObjectsMap::iterator it;
	for (it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
	{
		if (it->second->rayTestToObject(mouseCoords))
		{
			it->second->buttonDown(mouseCoords);
			return true;
		}
	}
}

void UIWindow::buttonDown(glm::vec2& mousePos)
{
	mouseDown_ = true;
}
void UIWindow::buttonUp(glm::vec2& mousePos)
{
	mouseDown_ = false;
	lastMouse = glm::vec2(-10.0);

	tWindowObjectsMap::iterator it;
	for (it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
	{
		it->second->buttonUp(mousePos);
	}
}

void UIWindow::drawChildrens(Shader* UIShader)
{
	tWindowObjectsMap::iterator it;
	for (it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
	{
		it->second->drawObject(UIShader);
	}
}
#include "UIDisplayer.h"
#include "UIWindow.h"

UIDisplayer::UIDisplayer(std::string name, SceneManager* manager)
{
	mName = name;
	mCurrentSceneManager = manager;
}


UIDisplayer::~UIDisplayer()
{
	tWindowsMap::iterator it;
	for (it = mWindowsMap.begin(); it != mWindowsMap.end(); ++it)
	{
		delete it->second;
	}
	mWindowsMap.clear();
}

UIWindow* UIDisplayer::createWindow(std::string name)
{
	tWindowsMap::iterator it = mWindowsMap.find(name);
	if (it != mWindowsMap.end())
	{
		return it->second;
	}

	UIWindow* newWindow = new UIWindow(name, mCurrentSceneManager);

	mWindowsMap.insert(std::pair<std::string, UIWindow*>(name, newWindow));
	return newWindow;
}
void UIDisplayer::deleteWindow(std::string name)
{
	tWindowsMap::iterator it = mWindowsMap.find(name);
	if (it != mWindowsMap.end())
	{
		delete it->second;
		mWindowsMap.erase(it);
	}
}
UIWindow* UIDisplayer::getWindow(std::string name)
{
	tWindowsMap::iterator it = mWindowsMap.find(name);
	if (it != mWindowsMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UIDisplayer::drawDisplayer(Shader* UIShader)
{
	tWindowsMap::iterator it;
	for (it = mWindowsMap.begin(); it != mWindowsMap.end(); ++it)
	{
		it->second->drawObject(UIShader);
	}
}
#include <iostream>

UIWindow* UIDisplayer::selectWindow_byCoords(glm::vec2 mouseCoords)
{
	tWindowsMap::iterator it;
	for (it = mWindowsMap.begin(); it != mWindowsMap.end(); ++it)
	{
		//If we are inside the AABB
		if (it->second->rayTestToObject(mouseCoords))
		{
			if (it->second->rayTestToChilds(mouseCoords))
			{
			}
			else
			{
				it->second->buttonDown(mouseCoords);
			}
			it->second->setSelected(true);
			return it->second;
		}
	}

	return NULL;
}

void UIDisplayer::mouseButtonUp(glm::vec2 mouseCoords)
{
	tWindowsMap::iterator it;
	for (it = mWindowsMap.begin(); it != mWindowsMap.end(); ++it)
	{
		it->second->buttonUp(mouseCoords);
	}
}
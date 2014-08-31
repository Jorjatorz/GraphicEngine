#include "UIDisplayer.h"
#include "UIWindow.h"

UIDisplayer::UIDisplayer(std::string name)
{
	mName = name;
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

	UIWindow* newWindow = new UIWindow(name);

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
		return nullptr;
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
#include "UIDisplayer.h"

#include "UIWindow.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "UICallbackListener.h"

UIDisplayer::UIDisplayer(std::string name, SceneManager* manager, UIManager* uiManager)
{
	mName = name;
	mCurrentManager = manager;
	mFocusWindow = NULL;
	mUIManager = uiManager;
}


UIDisplayer::~UIDisplayer()
{
	tWindowMap::iterator it;
	for (it = mWindowMap.begin(); it != mWindowMap.end(); ++it)
	{
		delete it->second;
	}
	mWindowMap.clear();
}

UIWindow* UIDisplayer::createUIWindow(std::string name, real Width, real Height, std::string uiFilePath)
{
	tWindowMap::iterator it = mWindowMap.find(name);
	if (it != mWindowMap.end())
	{
		return it->second;
	}

	UIWindow* newWindow = new UIWindow(name, Width, Height, uiFilePath, mCurrentManager);
	mFocusWindow = newWindow;
	newWindow->mAwesomiumView->Focus();
	mWindowMap.insert(std::pair<std::string, UIWindow*>(name, newWindow));

	return newWindow;

}
void UIDisplayer::deleteUIWindow(std::string name)
{
	tWindowMap::iterator it = mWindowMap.find(name);
	if (it != mWindowMap.end())
	{
		delete it->second;
		mWindowMap.erase(it);
	}
}
UIWindow* UIDisplayer::getWindow(std::string name)
{
	tWindowMap::iterator it = mWindowMap.find(name);
	if (it != mWindowMap.end())
	{
		return it->second;
	}

	return NULL;
}

void UIDisplayer::renderDisplayer()
{
	Shader* shad = mCurrentManager->createShader("UIShader", "UIShader");
	mCurrentManager->bindShader(shad);

	//Update the core
	mUIManager->update();

	tWindowMap::iterator it;
	for (it = mWindowMap.begin(); it != mWindowMap.end(); ++it)
	{
		Awesomium::WebCore::instance()->Update();
		it->second->render(shad);
	}
}

void UIDisplayer::setMouseMove(glm::vec2 newPos)
{
	tWindowMap::iterator it;
	for (it = mWindowMap.begin(); it != mWindowMap.end(); ++it)
	{
		if (it->second->rayTestToQuad(mCurrentManager->getMousePosition_NDC()))
		{
			it->second->setMouseMove(newPos);
			mFocusWindow = it->second;
			mFocusWindow->mAwesomiumView->Focus();
		}
		else if (mFocusWindow != NULL)
		{
			mFocusWindow->mAwesomiumView->Unfocus();
			mFocusWindow = NULL;
		}
	}
}
void UIDisplayer::setMouseButtonDown()
{
	if (mFocusWindow != NULL)
	{
		mFocusWindow->setMouseButtonDown();
	}
}
void UIDisplayer::setMouseButtonUp()
{
	if (mFocusWindow != NULL)
	{
		mFocusWindow->setMouseButtonUp();
	}
}

void UIDisplayer::setPropertyToWindow(std::string windowName, std::string elementName, std::string jsPropertyName, real value)
{
	UIWindow* wind = getWindow(windowName);

	if (wind != NULL)
	{
		wind->getCallBackListener()->setJSValue(elementName, jsPropertyName, value);
	}
}

void UIDisplayer::setPropertyToWindow(std::string windowName, std::string elementName, std::string jsPropertyName, std::string value)
{
	UIWindow* wind = getWindow(windowName);

	if (wind != NULL)
	{
		wind->getCallBackListener()->setJSValue(elementName, jsPropertyName, value);
	}
}

void UIDisplayer::setPropertyToWindow(std::string windowName, std::string elementName, std::string jsPropertyName, bool value)
{
	UIWindow* wind = getWindow(windowName);

	if (wind != NULL)
	{
		wind->getCallBackListener()->setJSValue(elementName, jsPropertyName, value);
	}
}

Awesomium::JSValue UIDisplayer::getPropertyFromWindow(std::string windowName, std::string elementName, std::string jsPropertyName)
{
	UIWindow* wind = getWindow(windowName);

	if (wind != NULL)
	{
		return wind->getCallBackListener()->getJSValue(elementName, jsPropertyName);
	}

	return Awesomium::JSValue::Undefined();
}
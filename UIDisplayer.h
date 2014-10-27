#ifndef UIDISPLAYER_H
#define UIDISPLAYER_H

#include "Definitions.h"

#include <string>
#include <map>

class UIDisplayer
{
public:
	UIDisplayer(std::string name, SceneManager* manager, UIManager* uiManager);
	~UIDisplayer();

	UIWindow* createUIWindow(std::string name, real Width, real Height, std::string uiFilePath);
	void deleteUIWindow(std::string name);
	UIWindow* getWindow(std::string name);
	UIWindow* getFocusWindow()
	{
		return mFocusWindow;
	}

	void renderDisplayer();

	//JS
	Awesomium::JSValue getPropertyFromWindow(std::string windowName, std::string elementName, std::string jsPropertyName);
	void setPropertyToWindow(std::string windowName, std::string elementName, std::string jsPropertyName, real value);
	void setPropertyToWindow(std::string windowName, std::string elementName, std::string jsPropertyName, std::string value);
	void setPropertyToWindow(std::string windowName, std::string elementName, std::string jsPropertyName, bool value);

	//Input
	void setMouseMove(glm::vec2 newPos);
	void setMouseButtonDown();
	void setMouseButtonUp();


private:
	std::string mName;
	SceneManager* mCurrentManager;
	UIManager *mUIManager;

	UIWindow* mFocusWindow;

	typedef std::map<std::string, UIWindow*> tWindowMap;
	tWindowMap mWindowMap;
};

#endif
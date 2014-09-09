#ifndef UIDISPLAYER_H
#define UIDISPLAYER_H

#include "Definitions.h"

#include <map>
#include <string>

class UIDisplayer
{
public:
	UIDisplayer(std::string name, SceneManager* manager);
	~UIDisplayer();

	void drawDisplayer(Shader* UIShader); //Draw all windows and its elements

	UIWindow* selectWindow_byCoords(glm::vec2 mouseCoords);
	void mouseButtonUp(glm::vec2 mouseCoords);

	UIWindow* createWindow(std::string name);
	void deleteWindow(std::string name);
	UIWindow* getWindow(std::string name);

private:
	std::string mName;

	typedef std::map<std::string, UIWindow*> tWindowsMap;
	tWindowsMap mWindowsMap;

	SceneManager* mCurrentSceneManager;
};

#endif
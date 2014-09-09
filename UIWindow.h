#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "Definitions.h"
#include "UIObject.h"

#include <string>
#include <map>

class UIWindow: public UIObject
{
public:
	UIWindow(std::string name, SceneManager* manager);
	~UIWindow();

	void drawChildrens(Shader* UIShader);

	//Button
	UIButton* createButton(std::string name);
	//Label
	UILabel* createLabel(std::string name);
	void deleteLabel(std::string name);

	void deleteObject(std::string name);

	void update();
	bool rayTestToChilds(glm::vec2 mouseCoords);
	void buttonDown(glm::vec2& mousePos);
	void buttonUp(glm::vec2& mousePos);

private:
	glm::vec2 lastMouse;

	typedef std::map<std::string, UIObject*> tWindowObjectsMap;
	tWindowObjectsMap mObjectsMap;
};

#endif
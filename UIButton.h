#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "Definitions.h"
#include "UIObject.h"

#include <string>
#include <iostream>

class UIButton: public UIObject
{
public:
	UIButton(std::string name, UIWindow* window, SceneManager* manager);
	~UIButton();

	void setFunctionWhen_ButtonUp(void (EventFunction::*function)());
	void update();

	void buttonDown(glm::vec2& mousePos);
	void buttonUp(glm::vec2& mousePos);

private:
	UIWindow* mParentWindow;

	//Pointer to a funcion when button is released
	void (EventFunction::*mButtonUpFunc)();

	glm::vec2 mouseDownCoords_, mouseUpCoords_;
};

#endif
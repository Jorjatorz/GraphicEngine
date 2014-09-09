#include "UIButton.h"

#include "UIWindow.h"
#include "EventFunction.h"

UIButton::UIButton(std::string name, UIWindow* window, SceneManager* manager)
{
	mName = name;
	mParentWindow = window;
	mCurrentSceneManager = manager;

	mPosition = glm::vec2(0.0);
	mSize = glm::vec2(0.25);

	mParentPosition = window->getPosition();
	mParentSize = window->getSize();

	mouseDownCoords_ = mouseUpCoords_ = glm::vec2(-10.0);
	mouseDown_ = false;

	mButtonUpFunc = NULL;
}


UIButton::~UIButton()
{
}


void UIButton::update()
{
	//Get the parent position
	mParentPosition = mParentWindow->getPosition();
	mParentSize = mParentWindow->getSize();
}

void UIButton::buttonDown(glm::vec2& mousePos)
{
	mouseDownCoords_ = mousePos;
	mouseDown_ = true;

	//Do event on instant click??
}
void UIButton::buttonUp(glm::vec2& mousePos)
{
	mouseUpCoords_ = mousePos;

	//Do event on release click??
	if (mouseDown_ && rayTestToObject(mouseUpCoords_)) //If the button was pressed and released inside the object
	{
		if (mButtonUpFunc != NULL)
		{
			EventFunction e;
			(e.*mButtonUpFunc)();
		}
	}

	mouseDown_ = false;
}

void UIButton::setFunctionWhen_ButtonUp(void (EventFunction::*function)())
{
	mButtonUpFunc = function;
}
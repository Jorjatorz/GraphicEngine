#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <iostream>

#include "Definitions.h"
#include "Singleton.h"

class InputManager : public Singleton<InputManager>
{
public:
	InputManager(void);
	~InputManager(void);

	//call per frame, check the events
	void getFrameInput(bool &running);

	//return if the key is pressed or not (scanCode)(mouse_button_)
	bool isKeyDown(short int keyCode);
	bool isKeyUp(short int keyCode);
	bool isMouseButtonDown(short int buttonCode);
	bool isMouseButtonUp(short int buttonCode);

	//Aux
	void setSDLWindow(SDL_Window* newWindow)
	{
		mSDLWindow = newWindow;
	}
	void setMousePosition(int x, int y);
	void getMousePosition(int &x, int &y);
	glm::vec2 getMousePosition_NDC();

private:
	//Array containing all the keys status (false if not pressed). Initialize to false, use SDL_Scancode
	bool mKeyPressedArray[SDL_NUM_SCANCODES];
	bool mMousePressedArray[3]; // 1: left, 2: middle, 3: right
	int mouseX, mouseY;

	//SDL varaibles
	SDL_Event mEvent;
	SDL_Window* mSDLWindow;
};

#endif
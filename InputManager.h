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

	//return if the key is pressed or not (scanCode)
	bool isKeyDown(short int keyCode);
	bool isKeyUp(short int keyCode);

	//Aux
	void setSDLWindow(SDL_Window* newWindow)
	{
		mSDLWindow = newWindow;
	}
	void InputManager::warpMouse(int x, int y);
	void InputManager::getMousePosition(int &x, int &y);

private:
	//Array containing all the keys status (false if not pressed). Initialize to false, use SDL_Scancode
	bool mKeyPressedArray[SDL_NUM_SCANCODES];

	//SDL varaibles
	SDL_Event mEvent;
	SDL_Window* mSDLWindow;
};

#endif
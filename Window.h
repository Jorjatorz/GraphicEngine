#ifndef WINDOW_H
#define WINDOW_H

#include "Definitions.h"

#include <string>

class Window
{
public:
	Window(std::string windowName, int width, int height, bool fullscreen);
	~Window(void);

	//Public functions

	//swap buffers
	void swapBuffers(bool swap);

	//Get functions
	int getWidth();
	int getHeight();
	bool isFullScreen();

private:
	//variables
	int mWindowWidth, mWindowHeight;
	bool mFullScreen;

	//SDL variables
	SDL_Window* mSDLWindow;
	SDL_GLContext mSDLContext;


	//Private functions

	//creates a window and set the new variables
	void initWindow(std::string windowName, int width, int height, bool fullscreen);

	//NON COPIABLE
	//Window(void);
	//Window(const Window &);
	//Window& operator=(const Window&);
};

#endif
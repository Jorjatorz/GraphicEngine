#ifndef WINDOW_H
#define WINDOW_H

#include "Definitions.h"

#include <string>

class Window
{
public:
	Window(std::string windowName, real width, real height, bool fullscreen);
	~Window(void);

	//Public functions

	//swap buffers
	void swapBuffers(bool swap);

	//Get functions
	real getWidth();
	real getHeight();
	bool isFullScreen();

private:
	//variables
	real mWindowWidth, mWindowHeight;
	bool mFullScreen;

	//SDL variables
	SDL_Window* mSDLWindow;
	SDL_GLContext mSDLContext;


	//Private functions

	//creates a window and set the new variables
	void initWindow(std::string windowName, real width, real height, bool fullscreen);

	//NON COPIABLE
	//Window(void);
	//Window(const Window &);
	//Window& operator=(const Window&);
};

#endif
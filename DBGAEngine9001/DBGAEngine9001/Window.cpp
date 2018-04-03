#include "stdafx.h"
#include "Window.h"

#include <assert.h>
#include <iostream>

Window::Window(
	const std::string & name,
	unsigned int xPos,
	unsigned int yPos,
	unsigned int width,
	unsigned int height,
	Uint32 flags
)
{
	window = SDL_CreateWindow(
		name.c_str(),
		xPos,
		yPos,
		width,
		height,
		flags
	);

	// If window is nullptr it means that no window has been created.
	// Outputs the message with SDL_GetError() that gets the last error caught by SDL.
	if (!window)
	{
		std::cout << "Errors in window's creation: " << SDL_GetError() << std::endl;
		assert(nullptr);
	}

	// Initializing surface...
	surface = SDL_GetWindowSurface(window);
}

Window::~Window()
{
	//SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
}

const std::pair<int, int> Window::GetSize() const
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return std::pair<int, int>(w, h);
}

SDL_Surface * const Window::GetSurface() const
{
	return surface;
}

const void Window::UpdateWindow()
{
	SDL_UpdateWindowSurface(window);
}
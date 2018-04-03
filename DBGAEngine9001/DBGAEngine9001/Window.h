#pragma once

#include <string>
#include <SDL.h>

class Window
{
private:
	SDL_Window * window = nullptr;
	SDL_Surface * surface = nullptr;

public:
	Window(
		const std::string & name,
		unsigned int xPos,
		unsigned int yPos,
		unsigned int width,
		unsigned int height,
		Uint32 flags
	);
	~Window();

	const std::pair<int, int> GetSize() const;

	SDL_Surface * const GetSurface() const;
	const void UpdateWindow();
};


// DBGAEngine9001.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "assert.h"
#include <string>
#include <sstream>
#include <stdint.h>
#include <mutex>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <SDL_main.h>
#include "CommandManager.h"
#include "Window.h"


#pragma region Struct "ImageInfo"
struct ImageInfo
{
	unsigned int Width = 0;
	unsigned int Height = 0;
};
#pragma endregion

#pragma region Struct "Color"
struct Color
{
	uint8_t Red = 0;
	uint8_t Green = 0;
	uint8_t Blue = 0;
};
#pragma endregion

#pragma region Class "Image"
class Image
{
public:
	Image(const ImageInfo & Info, const std::string & ImageName);
	~Image();

	const ImageInfo & GetInfo() const;

	bool SetPixel(unsigned int X, unsigned int Y, const Color & Color);
	const Color * GetPixel(unsigned int X, unsigned int Y) const;

	void Print() const;
	bool Save();

	const Color & operator[](const std::pair<unsigned int, unsigned int> & pair) const;
	Color & operator[](const std::pair<unsigned int, unsigned int> & pair);
private:
	ImageInfo Info{};
	std::string ImageName;
	//Color* Data;
	std::vector<Color> Data;
	std::mutex m;
};

Image::Image(const ImageInfo & Info, const std::string & ImageName) : Info(Info), ImageName(ImageName), Data(Info.Width * Info.Height)
{
	assert(
		!ImageName.empty() &&
		Info.Width > 0 &&
		Info.Height > 0
	);

	//Data = new Color[Info.Width * Info.Height];
}
Image::~Image()
{
	//delete[] Data;
}

const ImageInfo & Image::GetInfo() const { return Info; }

bool Image::SetPixel(unsigned int X, unsigned int Y, const Color & Color)
{
	if (X >= Info.Width || Y >= Info.Height) return false;

	Data[Y * Info.Width + X] = Color;
	return true;
}
const Color * Image::GetPixel(unsigned int X, unsigned int Y) const
{
	const Color * temp = nullptr;

	if (X < Info.Width && Y < Info.Height)
	{
		temp = &Data[Y * Info.Width + X];
	}

	return temp;
}

void Image::Print() const
{
	for (unsigned int y = 0; y < Info.Height; y++)
	{
		for (unsigned int x = 0; x < Info.Width; x++)
		{
			const Color * c = GetPixel(x, y);
			std::cout << "    " << (unsigned int)c->Red << " " << (unsigned int)c->Green << " " << (unsigned int)c->Blue;
		}

		std::cout << std::endl;
	}
}
bool Image::Save()
{
	std::ofstream Save(ImageName + ".pnm", std::ofstream::out);

	Save << "P3" << std::endl;
	Save << "# " << ImageName << ".pnm" << std::endl;
	Save << Info.Width << " " << Info.Height << std::endl;
	Save << "255" << std::endl;

	for (unsigned int y = 0; y < Info.Height; y++)
	{
		for (unsigned int x = 0; x < Info.Width; x++)
		{
			//std::cout << "[R:" << (int)Data[y * Info.Width + x].Red << ",G:" << (int)Data[y * Info.Width + x].Green << ",B:" << (int)Data[y * Info.Width + x].Blue << "] ";
			const Color * c = GetPixel(x, y);
			Save << " " << (unsigned int)c->Red << " " << (unsigned int)c->Green << " " << (unsigned int)c->Blue;
		}

		Save << std::endl;
	}

	Save.close();

	return true;
}

const Color & Image::operator[](const std::pair<unsigned int, unsigned int> & pair) const {
	assert(
		pair.first < Info.Width &&
		pair.second < Info.Height
	);

	//return *GetPixel(pair.first, pair.second);
	return Data[pair.second * Info.Width + pair.first];
}
Color & Image::operator[](const std::pair<unsigned int, unsigned int> & pair) {
	assert(
		pair.first < Info.Width &&
		pair.second < Info.Height
	);

	//return GetPixel(pair.first, pair.second);
	return Data[pair.second * Info.Width + pair.first];
}
#pragma endregion

// Variables
bool IsRunning = true;

// Functions
int drawAndSaveImage()
{
	int maxWidth = 800;
	int maxHeight = 600;

	ImageInfo MyInfo{ maxWidth, maxHeight };

	std::cout << "Istanziazione..." << std::endl;
	Image MyImg{ MyInfo, "MyGoodImgBW" };

	// Customization...
	std::cout << "Customizzazione..." << std::endl;

	float currentPixelColor;
	for (unsigned int y = 0; y < maxHeight; y++)
	{
		for (unsigned int x = 0; x < maxWidth; x++)
		{
			currentPixelColor = (x*y) * 256 / (maxWidth * maxHeight);

			MyImg.SetPixel(x, y, { (uint8_t)currentPixelColor, (uint8_t)currentPixelColor, (uint8_t)currentPixelColor });
		}
	}

	std::cout << "Inizio salvataggio..." << std::endl;

	// End of customization.
	// Save...
	if (MyImg.Save())
	{
		std::cout << "Salvato!" << std::endl;
	}

	//MyImg.Print();

	getchar();

	return 0;
}

void Ciao()
{
	std::cout << "Ciao!" << std::endl;
}
void Ciaone()
{
	std::cout << "MEGACIAONEGIGANTE!" << std::endl;
}
void Quit()
{
	IsRunning = false;
}

// Threads functions
void f1()
{
	int i = 0;
	std::cout << "AAA " << std::this_thread::get_id() << " " << i << std::endl;
}

std::mutex cout_exec;
void f2(int * ids, int index)
{
	cout_exec.lock();
	ids[index] += index;
	std::cout << index << " " << std::this_thread::get_id() << " aaa! " << ids[index] << std::endl;
	cout_exec.unlock();
}

#pragma region Main
int main(int argv, char ** argc)
{
	// Threads
	int * ids = new int[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int i = 0; i < 10; i++)
	{
		std::thread t0(f2, ids, 0);
		std::thread t1(f2, ids, 1);
		std::thread t2(f2, ids, 2);
		std::thread t3(f2, ids, 3);
		std::thread t4(f2, ids, 4);
		std::thread t5(f2, ids, 5);
		std::thread t6(f2, ids, 6);
		std::thread t7(f2, ids, 7);
		std::thread t8(f2, ids, 8);
		std::thread t9(f2, ids, 9);

		t0.join();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
	}

	for (int i = 0; i < 10; i++) {
		std::cout << ids[i] << std::endl;
	}

	system("pause");
	return 0;

	/*
	CommandManager commandManager;

	commandManager.AddCommand("Ciaone", "Function that says MEGACIAONEGIGANTE.", Ciaone);
	commandManager.AddCommand("Quit", "Function that quits the application.", Quit);

	std::string cmd = "";

	std::map<std::string, std::string> commandCommands = commandManager.GetCommands();
	*/

	SDL_Init(SDL_INIT_EVERYTHING);

	Window window(
		"DBGA Command Manager",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_RESIZABLE
	);

	SDL_Surface * surface = nullptr;
	SDL_Surface * drawTarget = nullptr;

	SDL_Surface * newSurf = SDL_LoadBMP("images/sailor.bmp");
	assert(newSurf);

	// New panel
	std::pair<int, int> winSize = window.GetSize();
	surface = window.GetSurface();

	drawTarget = SDL_CreateRGBSurfaceWithFormat(0, winSize.first, winSize.second, 32, SDL_PIXELFORMAT_RGBA32);
	assert(drawTarget);

	SDL_Rect dstrect;

	dstrect.x = 800 / 2;
	dstrect.y = 600 / 2;
	dstrect.w = 32;
	dstrect.h = 32;

	uint32_t * pixels;

	int colorSeq = 0;

	while (IsRunning)
	{
		/*
		system("cls");


		for (std::map<std::string, std::string>::iterator element = commandCommands.begin(); element != commandCommands.end(); ++element)
		{
			std::cout << element->first << " ==> " << element->second << std::endl;
		}


		std::cout << "Write the command name here: ";
		std::cin >> cmd;
		std::cout << std::endl << "---------------------------------" << std::endl << std::endl;

		bool returnValue = commandManager.ProcessCommand(cmd);

		std::cout << std::endl << "---------------------------------" << std::endl << std::endl;
		std::cout << (returnValue ? "Command executed succesfully!" : "What the hell is that command? Gimme something good!") << std::endl << std::endl;
		system("pause");
		*/

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				std::cout << "Mouse Position: ( " << event.motion.x << ", " << event.motion.y << " )" << std::endl;
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "Mouse Button Down! ( " << event.button.button << " )" << std::endl;
				//std::cout << "Dimension: ( " << window.GetSize().first << ", " << window.GetSize().second << " )" << std::endl;
				break;
			case SDL_QUIT:
				Quit();
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					dstrect.x--;
					break;
				case SDLK_RIGHT:
					dstrect.x++;
					break;
				case SDLK_UP:
					dstrect.y--;
					break;
				case SDLK_DOWN:
					dstrect.y++;
					break;
				}
				break;
			default:
				std::cout << "Command Type: " << event.type << std::endl;
			}
		}

		////surface = SDL_CreateRGBSurface(0, winSize.first, winSize.second, 32, 0, 0, 0, 0);
		//SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 255, 0, 0));

		colorSeq++;
		if (colorSeq > 255) colorSeq = 0;

		struct Pixel
		{
			uint8_t R = 0;
			uint8_t G = 0;
			uint8_t B = 0;
			uint8_t A = 255;
		};

		// Locks the surface tor this thread.
		SDL_LockSurface(drawTarget);

		pixels = static_cast<uint32_t*>(drawTarget->pixels);

		for (unsigned int i = 0; i < winSize.first * winSize.second; i++)
		{

			Pixel * pixel = (Pixel *)(pixels + i);
			pixel->R = i * 255 / (winSize.first * winSize.second);
			pixel->A = 255;
		}

		// Unlocks the surface.
		SDL_UnlockSurface(drawTarget);

		// Locks the surface tor this thread.
		SDL_LockSurface(newSurf);

		struct PixelSenzaA
		{
			uint8_t B = 0;
			uint8_t G = 0;
			uint8_t R = 0;
		};

		PixelSenzaA * pixelsNuovi = static_cast<PixelSenzaA*>(newSurf->pixels);

		for (unsigned int i = 0; i < newSurf->w * newSurf->h; i++)
		{
			PixelSenzaA * pixel = (PixelSenzaA *)(pixelsNuovi + i);

			float monoPixel = 0.299 * pixel->B + 0.587 * pixel->G + 0.114 * pixel->R;

			pixel->B = colorSeq - monoPixel;
			pixel->G = colorSeq - monoPixel;
			pixel->R = colorSeq - monoPixel;
		}

		// Unlocks the surface.
		SDL_UnlockSurface(newSurf);

		SDL_BlitSurface(drawTarget, nullptr, surface, nullptr);

		SDL_BlitSurface(newSurf, nullptr, surface, &dstrect);

		// Updates the window with the last infos.
		window.UpdateWindow();
	}

	// Free the temp surface.
	SDL_FreeSurface(newSurf);
	SDL_FreeSurface(drawTarget);

	SDL_Quit();

	//system("pause");

	return 0;
}
#pragma endregion

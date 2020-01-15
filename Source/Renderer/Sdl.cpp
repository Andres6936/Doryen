#include <fstream>
#include <Image/Image.hpp>
#include <iostream>
#include "Renderer/Sdl.hpp"

// Construct

Doryen::SDL::SDL() : Renderer()
{

}

// Methods

void Doryen::SDL::setFps(const short value)
{
	if (value == 0)
	{
		minimunFrameLength = 0;
	}
	else
	{
		minimunFrameLength = 1000 / value;
	}
}

short Doryen::SDL::getFps() const
{
	return framePerSecond;
}

float Doryen::SDL::getLasFrameLength() const
{
	return lastFrameLength;
}

void Doryen::SDL::sleepMilli(int milliseconds)
{
	SDL_Delay(milliseconds);
}

int Doryen::SDL::getElapsedMilli() const
{
	return SDL_GetTicks();
}

float Doryen::SDL::getElapsedSeconds() const
{
	return SDL_GetTicks() * 1.0f / 1000.0f;
}

void Doryen::SDL::onRenderer()
{
	if (hasInstanceActive)
	{
		// TODO: Throw Error
	}
	else
	{
		if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1)
		{
			// TODO: Throw Error
		}

		if (SDL_EnableKeyRepeat(
				SDL_DEFAULT_REPEAT_DELAY,
				SDL_DEFAULT_REPEAT_INTERVAL) == -1)
		{
			// TODO: Throw Error
		}

		createTablesOfCharacteres();

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

		std::string filename = "./libtcod.cfg";

		unsigned filesize = 0;
		unsigned char* buffer;

		std::ifstream stream;

		stream.open(filename, std::ifstream::binary);

		if (stream.is_open())
		{
			stream.seekg(0, std::ifstream::end);
			filesize = stream.tellg();
			stream.seekg(0, std::ifstream::beg);

			buffer = new unsigned char[filesize];

			stream.read((char*)buffer, filesize);

			// TODO: Parser file

			delete[] buffer;

			stream.close();
		}

		if (charmap == nullptr)
		{
			// reload the font when switching
			// renderer to restore original character colors
			loadFont();
		}

		// Create the buffer for render
		// characteres in the console
		createBuffer();

		if (isFullscreen())
		{
			findResolution();

			screen = SDL_SetVideoMode((int)getActualFullscreenWidth(), (int)getActualFullscreenHeigth(), 32,
					SDL_FULLSCREEN);

			setActualFullscreenWidth(getWidthInPixeles());
			setActualFullscreenHeigth(getHeigthInPixeles());

			SDL_FillRect(screen, nullptr, 0);
		}
			// No is fullscreen
		else
		{
			screen = SDL_SetVideoMode((int)(getWidth() * getFontWidth()), (int)(getHeigth() * getFontHeigth()), 32, 0);
		}

		std::cout << "Using SDL Renderer.\n";

		SDL_EnableUNICODE(1);

		hasInstanceActive = true;
	}
}

void Doryen::SDL::loadFont()
{
	delete charmap;

	bool isTransparent = false;

	Image image = Image(getFontfile().c_str());

	charmap = image.imageData.getRepresentation();

	setFontWidth(charmap->w / getFontCharHorizontalSize());
	setFontHeigth(charmap->h / getFontCharVerticalSize());

	// allocated bool array for colored flags
	createTablesOfCharacteres();

	// figure out what kind of font we have
	// check if the alpha layer is actually used
	if (charmap->format->BytesPerPixel == 4)
	{
		std::cout << "32 Bits Font." << "\n\tChecking for Alpha Layer.\n";

		for (int x = 0; !isTransparent && x < charmap->w; ++x)
		{
			for (int y = 0; !isTransparent && y < charmap->h; ++y)
			{
				auto pixel =
						(unsigned char*)charmap->pixels + (y * charmap->pitch) + (x * charmap->format->BytesPerPixel);
				unsigned char alpha = *(pixel + charmap->format->Ashift / 8);

				if (alpha < 255)
				{
					isTransparent = true;
				}
			}
		}

		if (isTransparent)
		{
			std::cout << "\t\tAlpha Layer Present.\n";
		}
		else
		{
			std::cout << "\t\tAlpha Layer Not Present.\n";
		}
	}
	else if (charmap->format->BytesPerPixel != 3)
	{
		// Convert to 24 bits
		std::cout << "Font BPP < 24 Bits." << "\n\tConverting to 24 Bits.\n";

		SDL_Surface* temporal = ImageData::createNewSurface(charmap->w, charmap->h, false);
		SDL_BlitSurface(charmap, nullptr, temporal, nullptr);

		delete charmap;

		// Copy information
		charmap = temporal;
	}
	else
	{
		std::cout << "24 Bits Font.\n";
	}

	if (!isTransparent)
	{
		unsigned keyx = 0;
		unsigned keyy = 0;

		if (isFontHasDoryenLayout())
		{
			keyx = getFontWidth() / 2;
			keyy = getFontHeigth() / 2;
		}
		else if (isFontHasRowLayout())
		{
			keyx = ((int)' ' % getFontCharHorizontalSize()) * getFontWidth() + (getFontWidth() / 2);
			keyy = ((int)' ' / getFontCharHorizontalSize()) * getFontHeigth() + (getFontHeigth() / 2);
		}
		else
		{
			keyx = ((int)' ' / getFontCharVerticalSize()) * getFontWidth() + (getFontWidth() / 2);
			keyy = ((int)' ' % getFontCharVerticalSize()) * getFontHeigth() + (getFontHeigth() / 2);
		}

		auto pixel =
				(unsigned char*)charmap->pixels + (keyy * charmap->pitch) + (keyx * charmap->format->BytesPerPixel);


		Color FontKeyColor = Color(
				*(pixel + charmap->format->Rshift / 8),
				*(pixel + charmap->format->Gshift / 8),
				*(pixel + charmap->format->Bshift / 8));

		changeFontKeyColor(FontKeyColor);

		std::cout << "Key Color : " << FontKeyColor.r << " " <<
				  FontKeyColor.g << " " << FontKeyColor.b << "\n";

		if (!isFontGrayscale() && charmap->format->BytesPerPixel == 4)
		{
			// 32 bits font but alpha layer not used.
			// Convert to 24 bits (faster)

			std::cout << "32 Bits Font with No Alpha." << "\n\tConverting to Faster 24 Bits\n";

			SDL_Surface* temporal = ImageData::createNewSurface(charmap->w, charmap->h, false);
			SDL_BlitSurface(charmap, nullptr, temporal, nullptr);

			delete temporal;

			// Copy information
			charmap = temporal;
		}
	}

	// Detect colored tiles
	for (int i = 0; i < getFontCharHorizontalSize() * getFontCharVerticalSize(); ++i)
	{
		bool endLoop = false;

		unsigned cx = i % getFontCharHorizontalSize();
		unsigned cy = i / getFontCharHorizontalSize();

		for (int px = 0; !endLoop && px < getFontWidth(); ++px)
		{
			for (int py = 0; !endLoop && py < getFontHeigth(); ++py)
			{
				auto pixel = (unsigned char*)charmap->pixels + (cy * getFontHeigth() + py) * charmap->pitch
							 + (cx * getFontWidth() + px) * charmap->format->BytesPerPixel;

				unsigned char r = *(pixel + charmap->format->Rshift / 8);
				unsigned char g = *(pixel + charmap->format->Gshift / 8);
				unsigned char b = *(pixel + charmap->format->Bshift / 8);

				Color colorKey = Color(r, g, b);

				if (charmap->format->BytesPerPixel == 3)
				{
					// Ignore key color
					if (getFontKeyColor().equals(colorKey))
					{
						continue;
					}
				}

				if (r != g || g != b || b != r)
				{
					setCharacterColored(i, true);
					std::cout << "Character for ASCII Code " << i << " is Colored\n";
					endLoop = true;
				}
			}
		}
	}

	// Convert 24/32 bits greyscale to 32bits font with alpha layer
	if (!isTransparent && isFontGrayscale())
	{
		// Black on white font ?
		bool isInvert = getFontKeyColor().r > 128;

		// Convert the surface to 32 bits if needed
		if (charmap->format->BytesPerPixel != 4)
		{
			// Convert to 32 bits
			std::cout << "24 Bits Greyscale Font." << "\n\tConverting to 32 Bits.\n";

			SDL_Surface* temporal = ImageData::createNewSurface(charmap->w, charmap->h, true);
			SDL_BlitSurface(charmap, nullptr, temporal, nullptr);

			delete charmap;

			// Copy information
			charmap = temporal;
		}

		for (int i = 0; i < getFontCharHorizontalSize() * getFontCharVerticalSize(); ++i)
		{
			unsigned cx = i % getFontCharHorizontalSize();
			unsigned cy = i / getFontCharHorizontalSize();

			// Fill the surface with white (except colored tiles),
			// use alpha layer for characters

			for (unsigned x = cx * getFontWidth(); x < (cx + 1) * getFontWidth(); ++x)
			{
				for (unsigned y = cy * getFontHeigth(); y < (cy + 1) * getFontHeigth(); ++y)
				{
					if (!isCharacterColored(i))
					{
						auto pixel =
								(unsigned char*)charmap->pixels + (y * charmap->pitch) +
								(x * charmap->format->BytesPerPixel);

						unsigned char r = *(pixel + charmap->format->Rshift / 8);

						if (isInvert)
						{
							*(pixel + charmap->format->Ashift / 8) = 255 - r;
						}
						else
						{
							*(pixel + charmap->format->Ashift / 8) = r;
						}

						*(pixel + charmap->format->Rshift / 8) = 255;
						*(pixel + charmap->format->Gshift / 8) = 255;
						*(pixel + charmap->format->Bshift / 8) = 255;
					}
					else
					{
						auto pixel =
								(unsigned char*)charmap->pixels + (y * charmap->pitch) +
								(x * charmap->format->BytesPerPixel);

						unsigned char r = *(pixel + charmap->format->Rshift / 8);
						unsigned char g = *(pixel + charmap->format->Gshift / 8);
						unsigned char b = *(pixel + charmap->format->Bshift / 8);

						Color colorKey = Color(r, g, b);

						if (getFontKeyColor().equals(colorKey))
						{
							*(pixel + charmap->format->Ashift / 8) = 0;
						}
						else
						{
							*(pixel + charmap->format->Ashift / 8) = 255;
						}
					}
				}
			}
		}
	}

	Color fontKeyColor = getFontKeyColor();

	unsigned int SDLKey = SDL_MapRGB(charmap->format, fontKeyColor.r, fontKeyColor.g, fontKeyColor.b);

	unsigned int RGBMask = charmap->format->Rmask | charmap->format->Gmask | charmap->format->Bmask;

	// Remove the alpha part
	unsigned int nRGBMask = ~RGBMask;

	if (charmap->format->BytesPerPixel == 3)
	{
		SDL_SetColorKey(charmap, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDLKey);
	}

	fillCharacterColorWith(getFontKeyColor());
	fillCharacterDrawedWith(true);

	checkTableOfCharacteres();

	if (!isFontHasDoryenLayout())
	{
		// Apply standard ascii mapping
		if (isFontHasRowLayout())
		{
			// For font in row
			for (int i = 0; i < getMaxFontChars(); ++i)
			{
				setLayoutCharacter(i, i);
			}
		}
		else
		{
			// for font in column
			for (int i = 0; i < getMaxFontChars(); ++i)
			{
				unsigned fy = i % getFontCharVerticalSize();
				unsigned fx = i / getFontCharVerticalSize();

				setLayoutCharacter(i, fx + (fy * getFontCharHorizontalSize()));
			}
		}
	}
}

void Doryen::SDL::findResolution()
{
	unsigned wantedw = 0;

	unsigned wantedh = 0;

	if (getFullscreenWidth() > getWidth() * getFontWidth())
	{
		wantedw = getFullscreenWidth();
	}
	else
	{
		wantedw = getWidth() * getFontWidth();
	}

	if (getFullscreenHeigth() > getHeigth() * getFontHeigth())
	{
		wantedh = getFullscreenHeigth();
	}
	else
	{
		wantedh = getHeigth() * getFontHeigth();
	}

	setActualFullscreenWidth(wantedw);
	setActualFullscreenHeigth(wantedh);

	SDL_Rect** modes = SDL_ListModes(nullptr, SDL_FULLSCREEN);

	unsigned bestw = 999'999;

	unsigned besth = 999'999;

	if (modes != (SDL_Rect**)0 && modes != (SDL_Rect**)-1)
	{
		for (unsigned i = 0; modes[i]; ++i)
		{
			if (modes[i]->w >= wantedw && modes[i]->w <= bestw
				&& modes[i]->h >= wantedh && modes[i]->h <= besth
				&& SDL_VideoModeOK(modes[i]->w, modes[i]->h, 32, SDL_FULLSCREEN))
			{
				bestw = modes[i]->w;
				besth = modes[i]->h;
			}
		}
	}

	if (bestw != 999'999)
	{
		setActualFullscreenWidth(bestw);
		setActualFullscreenHeigth(besth);
	}
}

unsigned Doryen::SDL::getWidthInPixeles() const
{
	return screen->w;
}

unsigned Doryen::SDL::getHeigthInPixeles() const
{
	return screen->h;
}

void Doryen::SDL::setWindowTitle(const std::string& _title)
{
	SDL_WM_SetCaption(_title.c_str(), nullptr);
}

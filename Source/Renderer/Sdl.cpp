#include <fstream>
#include <iostream>

#include <Doryen/Image/Image.hpp>
#include "Doryen/Renderer/Sdl.hpp"

// Construct

Doryen::SDL::SDL() : Renderer()
{

}

Doryen::SDL::~SDL()
{
	delete screen;
	delete charmap;
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

/**
 * Function private to file.
 *
 * If the filter returns 1, then the event will be added to the internal queue.
 * If it returns 0, then the event will be dropped from the queue. This allows
 * selective filtering of dynamically.
 *
 * @note Events pushed onto the queue with SDL_PushEvent or SDL_PeepEvents do
 *  not get passed through the event filter.
 *
 * @param event Events generate for the user (generally, the press an key in
 *  the keyboard or mouse).
 *
 * @return True (1) for all event different of motion mouse.
 */
int filterMovementMouse(const SDL_Event* event)
{
	// We wanna filter all the movement mouse events
	// and dropped (deleted) from the queue for that its
	// wont be process and avoid that block the rest events.
	if (event->type == SDL_MOUSEMOTION)
	{
		return 0;
	}
	else
	{
		return 1;
	}
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
			setWindowInFullscreen();
		}
			// No is fullscreen
		else
		{
			screen = SDL_SetVideoMode((int)(getWidth() * getFontWidth()), (int)(getHeigth() * getFontHeigth()), 32, 0);
		}

		std::cout << "Using SDL Renderer.\n";

		SDL_EnableUNICODE(1);

		hasInstanceActive = true;

		// An problem with the support to event
		// generate for the mouse, is that SDL
		// have the option of process the events
		// of two ways, the first is added the
		// event generate in a queue and the second
		// is get the last event generate.

		// Actually the second of process the event
		// is used for Doryen and is that in the past
		// Libtcod used the first way of process event
		// but this have the problem that the events
		// generate in an frame concrete not was all
		// process at same time, and if this events
		// are much, the event can will process inside
		// of 15 or more frames later.

		// This change produce that the movement
		// relatives of mouse cannot be get, but
		// in other case, the event generate for the
		// mouse and keyboard can be get with a flow
		// const without affect the one to other.

		// The events generate for the movement mouse
		// are too many and can be fill the queue of only
		// events that not necessary will be process
		// sequentially if not immediately, for it, is
		// needed filter the events and allow only events
		// that NOT ARE ABOVE MOVEMENT MOUSE.
		SDL_SetEventFilter(filterMovementMouse);
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

	setSdlKey(SDL_MapRGB(charmap->format, fontKeyColor.r, fontKeyColor.g, fontKeyColor.b));

	setRgbMask(charmap->format->Rmask | charmap->format->Gmask | charmap->format->Bmask);

	// Remove the alpha part
	setNrgbMask(~getRgbMask());

	setSdlKey(getSdlKey() & getRgbMask());

	if (charmap->format->BytesPerPixel == 3)
	{
		SDL_SetColorKey(charmap, SDL_SRCCOLORKEY | SDL_RLEACCEL, getSdlKey());
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

void Doryen::SDL::onExit()
{
	SDL_Quit();
}

Doryen::Key Doryen::SDL::getKeyPressed()
{
	return keyPressed;
}

void Doryen::SDL::convertToGenericEvent(SDL_Event& event, Doryen::Key& key)
{
	SDL_KeyboardEvent* keyboard = &event.key;

	// Convert SDL Event to Doryen Event
	switch (keyboard->keysym.sym)
	{
	case SDLK_BACKSPACE:
		key.setKeyCode(KeyCode::BACKSPACE);
		break;
	case SDLK_TAB:
		key.setKeyCode(KeyCode::TAB);
		break;
	case SDLK_RETURN:
		key.setKeyCode(KeyCode::ENTER);
		break;
	case SDLK_PAUSE:
		key.setKeyCode(KeyCode::PAUSE);
		break;
	case SDLK_ESCAPE:
		key.setKeyCode(KeyCode::ESCAPE);
		break;
	case SDLK_SPACE:
		key.setKeyCode(KeyCode::SPACE);
		break;
	case SDLK_DELETE:
		key.setKeyCode(KeyCode::DELETE);
		break;

	case SDLK_0:
		key.setKeyCode(KeyCode::K_0);
		break;
	case SDLK_1:
		key.setKeyCode(KeyCode::K_1);
		break;
	case SDLK_2:
		key.setKeyCode(KeyCode::K_2);
		break;
	case SDLK_3:
		key.setKeyCode(KeyCode::K_3);
		break;
	case SDLK_4:
		key.setKeyCode(KeyCode::K_4);
		break;
	case SDLK_5:
		key.setKeyCode(KeyCode::K_5);
		break;
	case SDLK_6:
		key.setKeyCode(KeyCode::K_6);
		break;
	case SDLK_7:
		key.setKeyCode(KeyCode::K_7);
		break;
	case SDLK_8:
		key.setKeyCode(KeyCode::K_8);
		break;
	case SDLK_9:
		key.setKeyCode(KeyCode::K_9);
		break;

	case SDLK_KP0:
		key.setKeyCode(KeyCode::KP_0);
		break;
	case SDLK_KP1:
		key.setKeyCode(KeyCode::KP_1);
		break;
	case SDLK_KP2:
		key.setKeyCode(KeyCode::KP_2);
		break;
	case SDLK_KP3:
		key.setKeyCode(KeyCode::KP_3);
		break;
	case SDLK_KP4:
		key.setKeyCode(KeyCode::KP_4);
		break;
	case SDLK_KP5:
		key.setKeyCode(KeyCode::KP_5);
		break;
	case SDLK_KP6:
		key.setKeyCode(KeyCode::KP_6);
		break;
	case SDLK_KP7:
		key.setKeyCode(KeyCode::KP_7);
		break;
	case SDLK_KP8:
		key.setKeyCode(KeyCode::KP_8);
		break;
	case SDLK_KP9:
		key.setKeyCode(KeyCode::KP_9);
		break;

	case SDLK_KP_PERIOD:
		key.setKeyCode(KeyCode::KP_DEC);
		break;
	case SDLK_KP_DIVIDE:
		key.setKeyCode(KeyCode::KP_DIV);
		break;
	case SDLK_KP_MULTIPLY:
		key.setKeyCode(KeyCode::KP_MUL);
		break;
	case SDLK_KP_MINUS:
		key.setKeyCode(KeyCode::KP_SUB);
		break;
	case SDLK_KP_PLUS:
		key.setKeyCode(KeyCode::KP_ADD);
		break;
	case SDLK_KP_ENTER:
		key.setKeyCode(KeyCode::KP_ENTER);
		break;

	case SDLK_UP:
		key.setKeyCode(KeyCode::UP);
		break;
	case SDLK_DOWN:
		key.setKeyCode(KeyCode::DOWN);
		break;
	case SDLK_RIGHT:
		key.setKeyCode(KeyCode::RIGHT);
		break;
	case SDLK_LEFT:
		key.setKeyCode(KeyCode::LEFT);
		break;

	case SDLK_HOME:
		key.setKeyCode(KeyCode::HOME);
		break;
	case SDLK_END:
		key.setKeyCode(KeyCode::END);
		break;
	case SDLK_PAGEUP:
		key.setKeyCode(KeyCode::PAGE_UP);
		break;
	case SDLK_PAGEDOWN:
		key.setKeyCode(KeyCode::PAGE_DOWN);
		break;
	case SDLK_PRINT:
		key.setKeyCode(KeyCode::PRINT_SCREEN);
		break;

	case SDLK_F1:
		key.setKeyCode(KeyCode::F1);
		break;
	case SDLK_F2:
		key.setKeyCode(KeyCode::F2);
		break;
	case SDLK_F3:
		key.setKeyCode(KeyCode::F3);
		break;
	case SDLK_F4:
		key.setKeyCode(KeyCode::F4);
		break;
	case SDLK_F5:
		key.setKeyCode(KeyCode::F5);
		break;
	case SDLK_F6:
		key.setKeyCode(KeyCode::F6);
		break;
	case SDLK_F7:
		key.setKeyCode(KeyCode::F7);
		break;
	case SDLK_F8:
		key.setKeyCode(KeyCode::F8);
		break;
	case SDLK_F9:
		key.setKeyCode(KeyCode::F9);
		break;
	case SDLK_F10:
		key.setKeyCode(KeyCode::F10);
		break;
	case SDLK_F11:
		key.setKeyCode(KeyCode::F11);
		break;
	case SDLK_F12:
		key.setKeyCode(KeyCode::F12);
		break;
	}
}

void Doryen::SDL::setWindowInFullscreen()
{
	findResolution();

	screen = SDL_SetVideoMode((int)getActualFullscreenWidth(), (int)getActualFullscreenHeigth(), 32,
			SDL_FULLSCREEN);

	setActualFullscreenWidth(getWidthInPixeles());
	setActualFullscreenHeigth(getHeigthInPixeles());

	SDL_FillRect(screen, nullptr, 0);

	SDL_UpdateRect(screen, 0, 0, 0, 0);

	// Neccessary for methods that not set the variable manually.
	setFullscreen(true);
}

void Doryen::SDL::draw()
{
	// Bitmap point to screen
	SDL_Surface* bitmap = screen;

	bool trackChanges = getOldFade() == getFade();

	unsigned hdelta = 0;

	unsigned bpp = charmap->format->BytesPerPixel;

	if (bpp == 4)
	{
		hdelta = (charmap->pitch - getFontWidth() * bpp) / 4;
	}
	else
	{
		hdelta = (charmap->pitch - getFontWidth() * bpp);
	}

	static SDL_Surface* charmapBackup = nullptr;

	if (charmapBackup == nullptr)
	{
		charmapBackup = ImageData::createNewSurface(charmap->w, charmap->h, true);
		SDL_BlitSurface(charmap, nullptr, charmapBackup, nullptr);
	}

	for (int y = 0; y < getHeigth(); ++y)
	{
		for (int x = 0; x < getWidth(); ++x)
		{
			// Character to draw
			Char character = buffer[x + getWidth() * y];

			// Previous character drawed
			Char previousCharacter = oldBuffer[x + getWidth() * y];

			SDL_Rect sourceRect;
			SDL_Rect destinRect;

			if (character.getCf() == -1)
			{
				character.setCf(getCharacterInLayoutCharacteres(character.getC()));
			}

			character.setDirt(false);

			bool changed = true;

			if (trackChanges)
			{
				changed = false;

				if (character.isDirt() ||
					character.getC() != previousCharacter.getC() ||
					character.getCf() != previousCharacter.getCf() ||
					isCharacterUpdated(character.getC()) ||
					!character.getBackground().equals(previousCharacter.getBackground()) ||
					!character.getForeground().equals(previousCharacter.getForeground()))
				{
					changed = true;
				}
			}

			if (changed)
			{
				Color background = character.getBackground();

				destinRect.x = x * getFontWidth();
				destinRect.y = y * getFontHeigth();

				destinRect.w = getFontWidth();
				destinRect.h = getFontHeigth();

				// Draw Background
				if (getFade() != 255)
				{
					short nr = (short)(background.r * getFade() / 255 + getFadingColor().r * (255 - getFade()) / 255);
					short ng = (short)(background.g * getFade() / 255 + getFadingColor().g * (255 - getFade()) / 255);
					short nb = (short)(background.b * getFade() / 255 + getFadingColor().b * (255 - getFade()) / 255);

					character.setBackground(Color(nr, ng, nb));

					// Overwrite background variable with the new color
					background = character.getBackground();
				}

				unsigned int SDLBack = SDL_MapRGB(bitmap->format, background.r, background.g, background.b);

				if (isFullscreen())
				{
					// TODO: Implemented
				}

				SDL_FillRect(bitmap, &destinRect, SDLBack);

				if (character.getC() != ' ')
				{
					// Draw Foreground
					Color foreground = character.getForeground();

					if (getFade() != 255)
					{
						short nr = (short)(foreground.r * getFade() / 255 +
										   getFadingColor().r * (255 - getFade()) / 255);
						short ng = (short)(foreground.g * getFade() / 255 +
										   getFadingColor().g * (255 - getFade()) / 255);
						short nb = (short)(foreground.b * getFade() / 255 +
										   getFadingColor().b * (255 - getFade()) / 255);

						character.setForeground(Color(nr, ng, nb));

						// Overwrite foreground variable with the new color
						foreground = character.getForeground();
					}

					// Only draw character if foreground color != background color
					if (isCharacterUpdated(character.getC()) ||
						!character.getBackground().equals(character.getForeground()))
					{
						if (charmap->format->Amask == 0 && foreground.equals(getFontKeyColor()))
						{
							// cannot draw with the key color...
							if (foreground.r < 255)
							{
								character.setForeground(Color(foreground.r += 1, foreground.g, foreground.b));

								// Overwrite the variable
								foreground = character.getForeground();
							}
							else
							{
								character.setForeground(Color(foreground.r -= 1, foreground.g, foreground.b));

								// Overwrite the variable
								foreground = character.getForeground();
							}
						}

						sourceRect.x = (character.getCf() % getFontCharHorizontalSize()) * getFontWidth();
						sourceRect.y = (character.getCf() / getFontCharHorizontalSize()) * getFontHeigth();
						sourceRect.w = getFontWidth();
						sourceRect.h = getFontHeigth();

						Color curtext = getColorInCharacterColorAt(character.getCf());

						if (isCharacterDrawed(character.getCf()) || !curtext.equals(character.getForeground()))
						{
							// change the character color in the font
							setCharacterDrawed(character.getCf(), false);

							unsigned int SDLFore =
									SDL_MapRGB(charmap->format, foreground.r, foreground.g, foreground.b) &
									getRgbMask();

							setColorInCharacterColorAt(character.getCf(), character.getForeground());

							if (bpp == 4)
							{
								// 32 bits font : fill the whole character with foreground color
								Uint32* pix = (Uint32*)(((Uint8*)charmap->pixels) + sourceRect.x * bpp +
														sourceRect.y * charmap->pitch);

								int h = (int)getFontHeigth();

								if (!isCharacterColored(character.getCf()))
								{
									while (h--)
									{
										int w = (int)getFontWidth();

										while (w--)
										{
											(*pix) &= getNrgbMask();
											(*pix) |= SDLFore;
											pix++;
										}

										pix += hdelta;
									}
								}
								else
								{
									// Colored character : multiply color with foreground color
									Uint32* pixorig = (Uint32*)(((Uint8*)charmapBackup->pixels) + sourceRect.x * bpp +
																sourceRect.y * charmapBackup->pitch);

									int hdeltaBackup = (int)(charmapBackup->pitch - getFontWidth() * 4) / 4;

									while (h > 0)
									{
										int w = (int)getFontWidth();

										while (w > 0)
										{
											int r = (int)*((Uint8*)pixorig + charmapBackup->format->Rshift / 8);
											int g = (int)*((Uint8*)pixorig + charmapBackup->format->Gshift / 8);
											int b = (int)*((Uint8*)pixorig + charmapBackup->format->Bshift / 8);

											// erase the color
											(*pix) &= getNrgbMask();

											r = r * foreground.r / 255;
											g = g * foreground.g / 255;
											b = b * foreground.b / 255;

											// set the new color
											(*pix) |= (r << charmap->format->Rshift) |
													  (g << charmap->format->Gshift) |
													  (b << charmap->format->Bshift);
											w--;
											pix++;
											pixorig++;
										}

										h--;
										pix += hdelta;
										pixorig += hdeltaBackup;
									}
								}
							}
							else
							{
								// 24 bits font : fill only non key color pixels
								Uint32* pix = (Uint32*)(((Uint8*)charmap->pixels) + sourceRect.x * bpp +
														sourceRect.y * charmap->pitch);

								int h = (int)getFontHeigth();

								if (!isCharacterColored(character.getCf()))
								{
									while (h--)
									{
										int w = (int)getFontWidth();

										while (w--)
										{
											if (((*pix) & getRgbMask()) != getSdlKey())
											{
												(*pix) &= getNrgbMask();
												(*pix) |= SDLFore;
											}

											pix = (Uint32*)((Uint8*)pix + 3);
										}

										pix = (Uint32*)((Uint8*)pix + hdelta);
									}
								}
								else
								{
									// Colored character : multiply color with foreground color
									Uint32* pixorig = (Uint32*)((Uint8*)charmapBackup->pixels + sourceRect.x * 4 +
																sourceRect.y * charmapBackup->pitch);

									// CharmapBackup is always 32 bits
									int hdeltaBackup = (int)(charmapBackup->pitch - getFontWidth() * 4) / 4;

									while (h > 0)
									{
										int w = (int)getFontWidth();

										while (w > 0)
										{
											if (((*pixorig) & getRgbMask()) != getSdlKey())
											{
												int r = (int)(*((Uint8*)(pixorig) +
																charmapBackup->format->Rshift / 8));
												int g = (int)(*((Uint8*)(pixorig) +
																charmapBackup->format->Gshift / 8));
												int b = (int)(*((Uint8*)(pixorig) +
																charmapBackup->format->Bshift / 8));

												// erase the color
												(*pix) &= getNrgbMask();

												r = r * foreground.r / 255;
												g = g * foreground.g / 255;
												b = b * foreground.b / 255;

												// set the new color
												(*pix) |= (r << charmap->format->Rshift) |
														  (g << charmap->format->Gshift) |
														  (b << charmap->format->Bshift);
											}

											w--;

											pix = (Uint32*)(((Uint8*)pix) + 3);
											pixorig++;
										}

										h--;

										pix = (Uint32*)(((Uint8*)pix) + hdelta);
										pixorig += hdeltaBackup;
									}
								}
							}
						}

						SDL_BlitSurface(charmap, &sourceRect, bitmap, &destinRect);
					}
				}
			}

			// Update the character
			setCharacterInBufferAt(x + getWidth() * y, character);
		}
	}

	// TODO: Render Callback Function

	SDL_Flip(screen);

	setOldFade(getFade());

	if (isAnyCharacterUpdated())
	{
		clearCharacterUpdate();
		resizeCharacterUpdate(getMaxFontChars());

		setAnyCharacterUpdated(false);
	}

	if (isFontUpdated())
	{
		clearCharacterUpdate();
		resizeCharacterUpdate(getMaxFontChars());
	}

	static unsigned oldTime = 0;
	static unsigned newTime = 0;
	static unsigned elapsed = 0;

	// Remember are static
	oldTime = newTime;

	newTime = SDL_GetTicks();

	int frameTime = 0;
	int timeToWait = 0;

	if (newTime / 1000 != elapsed)
	{
		// Update FPS every second
		setFramePerSeconds(getCurrentFramePerSeconds());
		setCurrentFramePerSeconds(0);

		elapsed = newTime / 1000;
	}

	// If too fast, wait
	frameTime = (int)(newTime - oldTime);

	setLastFrameLength((float)frameTime * 0.001f);

	setCurrentFramePerSeconds(getCurrentFramePerSeconds() + 1);

	timeToWait = (int)getMinimunFrameLength() - frameTime;

	if (oldTime > 0 && timeToWait > 0)
	{
		SDL_Delay(timeToWait);

		newTime = SDL_GetTicks();

		frameTime = (int)(newTime - oldTime);
	}

	setLastFrameLength((float)frameTime * 0.001f);

	fillOldBuffer();

	// TODO: The limit the frame per second (fps) of application too limit
	//  the velocity with the events are updated.

	// Update the Queue of events
	updateEventsQueue();
}

Doryen::Mouse Doryen::SDL::getMouseEvent()
{
	return mouse;
}

void Doryen::SDL::updateEventsQueue()
{
	SDL_PumpEvents();

	// Polls for currently pending events,
	// and returns true if there are any pending
	// events, or false if there are none available.

	// If 'event' is not NULL, the next event
	// is removed from the queue and stored in that area.
	eventPending = SDL_PollEvent(&event);

	processEventsOfExit();

	updateKeyEvents();
	updateMouseEvents();
}

void Doryen::SDL::processEventsOfExit()
{
	// Exist event that not are part of Mouse or Keyboard
	// but that are needed process, examples of this type
	// of events are the event of exit or termination
	// that happen when the user want exit of app.

	if (event.type == SDL_QUIT)
	{
		setRunning(false);
	}
}

void Doryen::SDL::updateKeyEvents()
{
	keyPressed.resetState();

	if (not eventPending) return;

	// Same that keyPressed
	if (event.type == SDL_KEYDOWN)
	{
		SDL_KeyboardEvent* keyboard = &event.key;

		// Has been pressed CTRL, ALT of SHIFT ?
		if (keyboard->keysym.sym == SDLK_LALT)
		{
			keyPressed.setKeyCode(KeyCode::ALT);
			keyPressed.setRigthAltPressed(true);
		}
		else if (keyboard->keysym.sym == SDLK_RALT)
		{
			keyPressed.setKeyCode(KeyCode::ALT);
			keyPressed.setLeftAltPressed(true);
		}
		else if (keyboard->keysym.sym == SDLK_LCTRL)
		{
			keyPressed.setKeyCode(KeyCode::CONTROL);
			keyPressed.setLeftCtrlPressed(true);
		}
		else if (keyboard->keysym.sym == SDLK_RCTRL)
		{
			keyPressed.setKeyCode(KeyCode::CONTROL);
			keyPressed.setRigthCtrlPressed(true);
		}
		else if (keyboard->keysym.sym == SDLK_LSHIFT ||
				 keyboard->keysym.sym == SDLK_RSHIFT)
		{
			keyPressed.setKeyCode(KeyCode::SHIFT);
			keyPressed.setShift(true);
		}

		// Convert the event of type SDL to a event of
		// type Doryen (Generic)
		convertToGenericEvent(event, keyPressed);

		keyPressed.setPressed(true);
	}
}

void Doryen::SDL::updateMouseEvents()
{
	// The mouse is an object that should be have
	// memory of past events, the aim is determine
	// if the user press ALT + Any Key in any moment
	// or if occur an mouse event while is pressed
	// the key ALT.

	// Although the mouse should be have memory,
	// exist events that should be reset, like the
	// movement made for the user.

	int coordinateX = 0;
	int coordinateY = 0;

	char buttonPressed = SDL_GetMouseState(&coordinateX, &coordinateY);

	mouse.setX(coordinateX);
	mouse.setY(coordinateY);

	const int CHART_WIDTH = getFontWidth();
	const int CHART_HEIGHT = getFontHeigth();

	mouse.setPositionCellX(mouse.getX() / CHART_WIDTH);
	mouse.setPositionCellY(mouse.getY() / CHART_HEIGHT);

	if (buttonPressed)
	{
		if (SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			mouse.setStatus(MouseCode::LEFT);
		}

		if (SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			mouse.setStatus(MouseCode::MIDDLE);
		}

		if (SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			mouse.setStatus(MouseCode::RIGHT);
		}
	}
}

#include <Doryen/Renderer/Sdl.hpp>
#include <Doryen/Renderer/Sdl/EventSdl.hpp>

// Construct

Doryen::SDL::SDL() : Renderer()
{

}

Doryen::SDL::~SDL()
{
	SDL_FreeSurface(charmap);
	SDL_Quit();
}

// Methods

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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	if (charmap == nullptr)
	{
		// reload the font when switching
		// renderer to restore original character colors
		loadFont();
	}

	// Create the buffer for render
	// characteres in the console
	createBuffer();

	SDL_EnableUNICODE(1);

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

void Doryen::SDL::loadFont()
{
	SDL_FreeSurface(charmap);

	bool isTransparent = false;

	image = std::move(ImageSdl(getFontfile().c_str()));

	charmap = image.getCopySurface();

	setFontWidth(charmap->w / getFontCharHorizontalSize());
	setFontHeight(charmap->h / getFontCharVerticalSize());

	// allocated bool array for colored flags
	createTablesOfCharacters();

	// figure out what kind of font we have
	// check if the alpha layer is actually used
	if (charmap->format->BytesPerPixel == 4)
	{
//		std::cout << "32 Bits Font." << "\n\tChecking for Alpha Layer.\n";

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
//			std::cout << "\t\tAlpha Layer Present.\n";
		}
		else
		{
//			std::cout << "\t\tAlpha Layer Not Present.\n";
		}
	}
	else if (charmap->format->BytesPerPixel != 3)
	{
		// Convert to 24 bits
//		std::cout << "Font BPP < 24 Bits." << "\n\tConverting to 24 Bits.\n";

		SDL_Surface* temporal = ImageSdl::createNewSurface(charmap->w, charmap->h, false);
		SDL_BlitSurface(charmap, nullptr, temporal, nullptr);

		delete charmap;

		// Copy information
		charmap = temporal;
	}
	else
	{
//		std::cout << "24 Bits Font.\n";
	}

	if (!isTransparent)
	{
		unsigned keyx = 0;
		unsigned keyy = 0;

		if (isFontHasDoryenLayout())
		{
			keyx = getFontWidth() / 2;
			keyy = getFontHeight() / 2;
		}
		else if (isFontHasRowLayout())
		{
			keyx = ((int)' ' % getFontCharHorizontalSize()) * getFontWidth() + (getFontWidth() / 2);
			keyy = ((int)' ' / getFontCharHorizontalSize()) * getFontHeight() + (getFontHeight() / 2);
		}
		else
		{
			keyx = ((int)' ' / getFontCharVerticalSize()) * getFontWidth() + (getFontWidth() / 2);
			keyy = ((int)' ' % getFontCharVerticalSize()) * getFontHeight() + (getFontHeight() / 2);
		}

		auto pixel =
				(unsigned char*)charmap->pixels + (keyy * charmap->pitch) + (keyx * charmap->format->BytesPerPixel);


		Color FontKeyColor = Color(
				*(pixel + charmap->format->Rshift / 8),
				*(pixel + charmap->format->Gshift / 8),
				*(pixel + charmap->format->Bshift / 8));

		changeFontKeyColor(FontKeyColor);

//		std::cout << "Key Color : " << FontKeyColor.r << " " <<
//				  FontKeyColor.g << " " << FontKeyColor.b << "\n";

		if (!isFontGrayscale() && charmap->format->BytesPerPixel == 4)
		{
			// 32 bits font but alpha layer not used.
			// Convert to 24 bits (faster)

//			std::cout << "32 Bits Font with No Alpha." << "\n\tConverting to Faster 24 Bits\n";

			SDL_Surface* temporal = ImageSdl::createNewSurface(charmap->w, charmap->h, false);
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
			for (int py = 0; !endLoop && py < getFontHeight(); ++py)
			{
				auto pixel = (unsigned char*)charmap->pixels + (cy * getFontHeight() + py) * charmap->pitch
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
//					std::cout << "Character for ASCII Code " << i << " is Colored\n";
					endLoop = true;
				}
			}
		}
	}

	// Convert 24/32 bits greyscale to 32bits font with alpha layer
	if (!isTransparent && isFontGrayscale())
	{
		// Black on white font ?
		bool isInvert = getFontKeyColor().getRed() > 128;

		// Convert the surface to 32 bits if needed
		if (charmap->format->BytesPerPixel != 4)
		{
			// Convert to 32 bits
//			std::cout << "24 Bits Greyscale Font." << "\n\tConverting to 32 Bits.\n";

			SDL_Surface* temporal = ImageSdl::createNewSurface(charmap->w, charmap->h, true);
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
				for (unsigned y = cy * getFontHeight(); y < (cy + 1) * getFontHeight(); ++y)
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

	setSdlKey(SDL_MapRGB(charmap->format, fontKeyColor.getRed(), fontKeyColor.g, fontKeyColor.b));

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

	// This method is possible that will be called more of one time, hence is important
	// that if the variable screen has been initialized it will be deleted for again
	// initialize correctly.
	if (screen not_eq nullptr) SDL_FreeSurface(screen);

	screen = SDL_SetVideoMode((int)(getWidth() * getFontWidth()), (int)(getHeight() * getFontHeight()), 32, 0);
}

void Doryen::SDL::setWindowTitle(const std::string& _title)
{
	SDL_WM_SetCaption(_title.c_str(), nullptr);
}

void
Doryen::SDL::coloredCharacter(const SDL_Rect& sourceRect,
		const SDL_Surface* charmapBackup, Uint32* pix,
		const std::uint32_t hdelta)
{
	// For images to color the bytes per pixel are 4.
	const std::int8_t BYTE_PER_PIXEL = 4;

	// Colored character : multiply color with foreground color
	Uint32* pixorig = (Uint32*)(((Uint8*)charmapBackup->pixels) +
								sourceRect.x * BYTE_PER_PIXEL +
								sourceRect.y * charmapBackup->pitch);

	int hdeltaBackup =
			(int)(charmapBackup->pitch - getFontWidth() * 4) / 4;

	int h = getFontHeight();

	while (h > 0)
	{
		int w = (int)getFontWidth();

		while (w > 0)
		{
			int r = (int)*((Uint8*)pixorig +
						   charmapBackup->format->Rshift / 8);
			int g = (int)*((Uint8*)pixorig +
						   charmapBackup->format->Gshift / 8);
			int b = (int)*((Uint8*)pixorig +
						   charmapBackup->format->Bshift / 8);

			// erase the color
			(*pix) &= getNrgbMask();

			const Color& foreground = getForeground();

			r = r * foreground.getRed() / 255;
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


void
Doryen::SDL::fillOnlyNonKeyColorPixels(const SDL_Rect& sourceRect,
		const SDL_Surface* charmapBackup,
		const std::uint32_t hdelta, const std::uint32_t bpp,
		const std::uint32_t SDLFore, const Char& character)
{
	// 24 bits font : fill only non key color pixels
	Uint32* pix = (Uint32*)(((Uint8*)charmap->pixels) + sourceRect.x * bpp +
							sourceRect.y * charmap->pitch);

	int h = (int)getFontHeight();

	if (!isCharacterColored(character.getCharacterFont()))
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
		Uint32* pixorig = (Uint32*)((Uint8*)charmapBackup->pixels +
									sourceRect.x * 4 +
									sourceRect.y * charmapBackup->pitch);

		// CharmapBackup is always 32 bits
		int hdeltaBackup =
				(int)(charmapBackup->pitch - getFontWidth() * 4) / 4;

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

					const Color& foreground = getForeground();

					r = r * foreground.getRed() / 255;
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

void Doryen::SDL::draw()
{
	// Bitmap point to screen
	SDL_Surface* bitmap = screen;

	unsigned hdelta = 0;

	// The number of bytes required to hold a pixel value
	// The data types used to represent pixels are as follows:
	// 1 Byte per pixel -> Uint8
	// 2 Byte per pixel -> Uint16
	// 3 Byte per pixel -> Tuple of Uint8 RGB values
	// 4 Byte per pixel -> Uint32
	const std::uint8_t BYTES_PER_PIXEL = charmap->format->BytesPerPixel;

	if (BYTES_PER_PIXEL == 4)
	{
		hdelta = (charmap->pitch - getFontWidth() * BYTES_PER_PIXEL) / 4;
	}
	else
	{
		hdelta = (charmap->pitch - getFontWidth() * BYTES_PER_PIXEL);
	}

	static SDL_Surface* charmapBackup = nullptr;

	if (charmapBackup == nullptr)
	{
		charmapBackup = ImageSdl::createNewSurface(charmap->w, charmap->h, true);
		SDL_BlitSurface(charmap, nullptr, charmapBackup, nullptr);
	}

	for (int y = 0; y < getHeight(); ++y)
	{
		for (int x = 0; x < getWidth(); ++x)
		{
			// Character to draw
			Char character = front[x + getWidth() * y];

			// Previous character drawed
			Char previousCharacter = back[x + getWidth() * y];

			SDL_Rect sourceRect;
			SDL_Rect destinRect;

			if (character.getCharacterFont() == -1)
			{
				character.setCharacterFont(
						getCharacterInLayoutCharacteres(character.getCharacter()));
			}

			bool changed = false;

			if (not character.equals(previousCharacter) or
				isCharacterUpdated(character.getCharacter()))
			{
				changed = true;
			}

			// An problem with rendering the callback is that not is
			// possible for Doryen detect an change in the characters,
			// because the callback work to level of pixels, so that
			// each that the callback is rendering it get the last screen
			// updated for Doryen, it derive in incosistences when the
			// callback try modified the pixeles of surface.
			if (changed or callbackRender not_eq nullptr)
			{
				Color background = character.getBackground();

				destinRect.x = x * getFontWidth();
				destinRect.y = y * getFontHeight();

				destinRect.w = getFontWidth();
				destinRect.h = getFontHeight();

				// Draw Background
				if (getFade() != 255)
				{
					short nr = (short)(background.getRed() * getFade() / 255 +
									   getFadingColor().r * (255 - getFade()) / 255);
					short ng = (short)(background.g * getFade() / 255 +
									   getFadingColor().g * (255 - getFade()) / 255);
					short nb = (short)(background.b * getFade() / 255 +
									   getFadingColor().b * (255 - getFade()) / 255);

					character.setBackground(Color(nr, ng, nb));

					// Overwrite background variable with the new color
					background.copy(character.getBackground());
				}

				unsigned int SDLBack = SDL_MapRGB(bitmap->format, background.getRed(), background.g,
						background.b);

				if (isFullscreen())
				{
					// TODO: Implemented
				}

				SDL_FillRect(bitmap, &destinRect, SDLBack);

				// Avoid draw the space character (A empty character)
				if (character.getCharacter() == ' ')
				{
					// Update the character and continue with the next Char
					setCharacterInBufferAt(x + getWidth() * y, character);
					continue;
				}

				// Draw Foreground
				Color foreground = character.getForeground();

				if (getFade() != 255)
				{
					short nr = (short)(foreground.getRed() * getFade() / 255 +
									   getFadingColor().getRed() * (255 - getFade()) / 255);
					short ng = (short)(foreground.g * getFade() / 255 +
									   getFadingColor().g * (255 - getFade()) / 255);
					short nb = (short)(foreground.b * getFade() / 255 +
									   getFadingColor().b * (255 - getFade()) / 255);

					character.setForeground(Color(nr, ng, nb));

					// Overwrite foreground variable with the new color
					foreground.copy(character.getForeground());
				}

				// Only draw character if foreground color != background color
				if (isCharacterUpdated(character.getCharacter()) ||
					!character.getBackground().equals(character.getForeground()))
				{
					if (charmap->format->Amask == 0 && foreground.equals(getFontKeyColor()))
					{
						// cannot draw with the key color...
						if (foreground.getRed() < 255)
						{
							foreground.addRed(1);
							character.setForeground(foreground);
						}
						else
						{
							foreground.addRed(-1);
							character.setForeground(foreground);
						}
					}

					sourceRect.x = (character.getCharacterFont() % getFontCharHorizontalSize()) *
								   getFontWidth();
					sourceRect.y = (character.getCharacterFont() / getFontCharHorizontalSize()) *
								   getFontHeight();
					sourceRect.w = getFontWidth();
					sourceRect.h = getFontHeight();

					Color curtext = getColorInCharacterColorAt(character.getCharacterFont());

					if (isCharacterDrawed(character.getCharacterFont()) ||
						!curtext.equals(character.getForeground()))
					{
						// change the character color in the font
						setCharacterDrawed(character.getCharacterFont(), false);

						unsigned int SDLFore =
								SDL_MapRGB(charmap->format, foreground.getRed(), foreground.g,
										foreground.b) &
								getRgbMask();

						setColorInCharacterColorAt(character.getCharacterFont(),
								character.getForeground());

						if (BYTES_PER_PIXEL == 4)
						{
							// 32 bits font : fill the whole character with foreground color
							Uint32* pix = (Uint32*)(((Uint8*)charmap->pixels) +
													sourceRect.x * BYTES_PER_PIXEL +
													sourceRect.y * charmap->pitch);

							int h = (int)getFontHeight();

							if (!isCharacterColored(character.getCharacterFont()))
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
								coloredCharacter(sourceRect, charmapBackup, pix, hdelta);
							}
						}
						else
						{
							fillOnlyNonKeyColorPixels(sourceRect, charmapBackup, hdelta,
									BYTES_PER_PIXEL, SDLFore, character);
						}
					}

					SDL_BlitSurface(charmap, &sourceRect, bitmap, &destinRect);
				}
			}

			// Update the character
			setCharacterInBufferAt(x + getWidth() * y, character);
		}
	}

	// Render Callback Function
	if (callbackRender) callbackRender->render(screen);

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

	timeToWait = (int)getMinimumFrameLength() - frameTime;

	if (oldTime > 0 && timeToWait > 0)
	{
		SDL_Delay(timeToWait);

		newTime = SDL_GetTicks();

		frameTime = (int)(newTime - oldTime);
	}

	setLastFrameLength((float)frameTime * 0.001f);

	fillOldBuffer();

	// Note: The limit the frame per second (fps) of application too limit
	//  the velocity with the events are updated.

	// Update the Queue of events
	updateEventsQueue();
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
			keyPressed.setRightAltPressed(true);
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
			keyPressed.setRightCtrlPressed(true);
		}
		else if (keyboard->keysym.sym == SDLK_LSHIFT ||
				 keyboard->keysym.sym == SDLK_RSHIFT)
		{
			keyPressed.setKeyCode(KeyCode::SHIFT);
			keyPressed.setShift(true);
		}

		// Convert the event of type SDL to a event of
		// type Doryen (Generic)
		EventSdl::getGenericEvent(event, keyPressed);

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

	mouse.resetState();

	int coordinateX = 0;
	int coordinateY = 0;

	// Ignore the result of function, already manage the events.
	SDL_GetMouseState(&coordinateX, &coordinateY);

	mouse.setX(coordinateX);
	mouse.setY(coordinateY);

	const int CHART_WIDTH = getFontWidth();
	const int CHART_HEIGHT = getFontHeight();

	mouse.setPositionCellX(mouse.getX() / CHART_WIDTH);
	mouse.setPositionCellY(mouse.getY() / CHART_HEIGHT);

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		SDL_MouseButtonEvent* mev = &event.button;

		switch (mev->button)
		{
		case SDL_BUTTON_LEFT :
			mouse.setStatus(MouseCode::LEFT);
			break;

		case SDL_BUTTON_MIDDLE :
			mouse.setStatus(MouseCode::MIDDLE);
			break;


		case SDL_BUTTON_RIGHT :
			mouse.setStatus(MouseCode::RIGHT);
			break;

		case SDL_BUTTON_WHEELUP:
			mouse.setStatus(MouseCode::WHEEL_UP);
			break;

		case SDL_BUTTON_WHEELDOWN:
			mouse.setStatus(MouseCode::WHEEL_DOWN);
			break;
		}
	}
}

void Doryen::SDL::showCursor(bool visible)
{
	SDL_ShowCursor(visible);
}

void Doryen::SDL::registerCallback(std::unique_ptr<CallbackRender> _callback)
{
	callbackRender = std::move(_callback);
}

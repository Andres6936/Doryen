// Joan Andrés (@Andres6936) Github.

#include <SDL/SDL.h>
#include <Doryen/Renderer/Sdl/MouseSdl.hpp>

using namespace Doryen;

void MouseSdl::updateGeneric(const EventSdl& eventSdl, Mouse& mouse) noexcept
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

	if (eventSdl.getUnderlineListener().type == SDL_MOUSEBUTTONDOWN)
	{
		const SDL_MouseButtonEvent* mev = &eventSdl.getUnderlineListener().button;

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

void MouseSdl::updateRelative(const Geometry::Size& sizeFont, Mouse& mouse) noexcept
{
	mouse.setPositionCellX(mouse.getX() / sizeFont.getWidth());
	mouse.setPositionCellY(mouse.getY() / sizeFont.getHeight());
}

// Joan Andrés (@Andres6936) Github.

#include "Mouse.hpp"

using namespace Doryen;

Functor::Mouse::Mouse(Console& _console) : ISample("Mouse support", _console)
{

}

void Functor::Mouse::render(KeyCode key, const Doryen::Mouse& mouse)
{
	drawBackground();

	sample.write(1, 1, format("Mouse position : {4d}x{4d}", mouse.getX(), mouse.getY()));

	sample.write(1, 2,
			format("Mouse cell     : {4d}x{4d}", mouse.getPositionCellX(), mouse.getPositionCellY()));

	sample.write(1, 4, format("Left button   : {}", mouse.isPressedLeftButton() ? "ON" : "OFF"));
	sample.write(1, 6, format("Middle button : {}", mouse.isPressedMiddleButton() ? "ON" : "OFF"));
	sample.write(1, 5, format("Right button  : {}", mouse.isPressedRightButton() ? "ON" : "OFF"));

	sample.write(1, 7, format("Wheel          : {}",
			mouse.isWheelUp() ? "UP" : (mouse.isWheelDown() ? "DOWN" : "")));

	sample.write(1, 10, format("Press TAB for {} cursor", visibleCursor ? "hidden" : "show"));

	if (key == KeyCode::TAB)
	{
		// Toggle the variable
		visibleCursor = not visibleCursor;
	}
}

void Functor::Mouse::drawBackground() const
{
	sample.setBackgroundColor(Palette::GRAY_WARN_30);
	sample.setForegroundColor(Palette::YELLOW);
	sample.showCursor(visibleCursor);

	sample.clear();
}

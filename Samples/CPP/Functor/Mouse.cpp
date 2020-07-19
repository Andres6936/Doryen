// Joan Andrés (@Andres6936) Github.

#include "Mouse.hpp"

using namespace Doryen;

Functor::Mouse::Mouse(std::string _name, Console& _console) : ISample(_name, _console)
{

}

void Functor::Mouse::render(KeyCode key, const Doryen::Mouse& mouse)
{
	drawBackground();

	sample.print(1, 1, format("Mouse position : {4d}x{4d}", mouse.getX(), mouse.getY()));

	sample.print(1, 2,
			format("Mouse cell     : {4d}x{4d}", mouse.getPositionCellX(), mouse.getPositionCellY()));

	sample.print(1, 4, format("Left button   : {}", mouse.isPressedLeftButton() ? "ON" : "OFF"));
	sample.print(1, 6, format("Middle button : {}", mouse.isPressedMiddleButton() ? "ON" : "OFF"));
	sample.print(1, 5, format("Right button  : {}", mouse.isPressedRightButton() ? "ON" : "OFF"));

	sample.print(1, 7, format("Wheel          : {}",
			mouse.isWheelUp() ? "UP" : (mouse.isWheelDown() ? "DOWN" : "")));

	sample.print(1, 10, format("Press TAB for {} cursor", visibleCursor ? "hidden" : "show"));

	if (key == KeyCode::TAB)
	{
		// Toggle the variable
		visibleCursor = not visibleCursor;
	}
}

void Functor::Mouse::drawBackground() const
{
	sample.setDefaultBackground(Color::GRAY_WARN_30);
	sample.setDefaultForeground(Color::ACCENT_WARN_LIGHT);
	sample.showCursor(visibleCursor);

	sample.clear();
}

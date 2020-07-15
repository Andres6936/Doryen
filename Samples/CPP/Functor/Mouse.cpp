// Joan Andrés (@Andres6936) Github.

#include "Mouse.hpp"

using namespace Doryen;

Functor::Mouse::Mouse(std::string _name, Console& _console) : ISample(_name, _console)
{

}

void Functor::Mouse::render(KeyCode key, const Doryen::Mouse& mouse)
{
	static bool lbut = false, rbut = false, mbut = false;

	drawBackground();

	if (mouse.isPressedLeftButton())
	{ lbut = !lbut; }
	if (mouse.isPressedRightButton())
	{ rbut = !rbut; }
	if (mouse.isPressedMiddleButton())
	{ mbut = !mbut; }

	sample.print(1, 1, format("Mouse position : {4d}x{4d}\n",
			mouse.getX(), mouse.getY()));

	sample.print(1, 2,
			format("Mouse cell     : {4d}x{4d}", mouse.getPositionCellX(), mouse.getPositionCellY()));

	sample.print(1, 4,
			format("Left button    : {} (toggle {})",
					mouse.isPressedLeftButton() ? " ON" : "OFF", lbut ? " ON" : "OFF"));

	sample.print(1, 5, format("Right button   : {} (toggle {})",
			mouse.isPressedRightButton() ? " ON" : "OFF", rbut ? " ON" : "OFF"));

	sample.print(1, 6, format("Middle button  : {} (toggle {})",
			mouse.isPressedMiddleButton() ? " ON" : "OFF", mbut ? " ON" : "OFF"));

//	sample.print(1, 7,format("Wheel          : {}",
//					mouse->wheel_up ? "UP" : (mouse->wheel_down ? "DOWN" : ""));

	sample.print(1, 10, format("Press TAB for {} cursor", visibleCursor ? "hidden" : "show"));

	if (key == KeyCode::TAB)
	{
		visibleCursor = not visibleCursor;
	}
}

void Functor::Mouse::drawBackground() const
{
	sample.setDefaultBackground(Color::grey);
	sample.setDefaultForeground(Color::lightYellow);
	sample.showCursor(visibleCursor);

	sample.clear();
}

// Joan Andrés (@Andres6936) Github.

#include "Mouse.hpp"


using namespace Doryen;

void Functor::Mouse::operator()(KeyCode key, const Doryen::Mouse& mouse)
{
	static bool lbut = false, rbut = false, mbut = false;

	static bool first = true;

	if (first)
	{
		first = false;

		Doryen::Platform::setFps(30); // fps limited to 30
		sample.setDefaultBackground(Doryen::Color::grey);
		sample.setDefaultForeground(Doryen::Color::lightYellow);
		TCODMouse::move(320, 200);
		TCODMouse::showCursor(true);
	}

	sample.clear();
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

	sample.print(1, 3,
			format("Mouse movement : {4d}x{4d}", mouse.getMovementRelativeX(), mouse.getMovementRelativeY()));

	sample.print(1, 4,
			format("Left button    : {} (toggle {})",
					mouse.isPressedLeftButton() ? " ON" : "OFF", lbut ? " ON" : "OFF"));

	sample.print(1, 5, format("Right button   : {} (toggle {})",
			mouse.isPressedRightButton() ? " ON" : "OFF", rbut ? " ON" : "OFF"));

	sample.print(1, 6, format("Middle button  : {} (toggle {})",
			mouse.isPressedMiddleButton() ? " ON" : "OFF", mbut ? " ON" : "OFF"));

//	sample.print(1, 7,format("Wheel          : {}",
//					mouse->wheel_up ? "UP" : (mouse->wheel_down ? "DOWN" : ""));

	sample.print(1, 10, "1 : Hide cursor\n2 : Show cursor");

//	if (key->c == '1')
//	{ TCODMouse::showCursor(false); }
//	else if (key->c == '2')
//	{ TCODMouse::showCursor(true); }
}

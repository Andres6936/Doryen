#include <string.h>
#include "libtcod.hpp"
#include "gui.hpp"

Doryen::Color Widget::back = Doryen::Color( 40, 40, 120 );

Doryen::Color Widget::fore = Doryen::Color( 220, 220, 180 );

Doryen::Color Widget::backFocus = Doryen::Color( 70, 70, 130 );

Doryen::Color Widget::foreFocus = Doryen::Color( 255, 255, 255 );

Doryen::Console *Widget::con = NULL;
TCODList<Widget *> Widget::widgets;
TCOD_mouse_t Widget::mouse;
float Widget::elapsed;
Widget *Widget::focus=NULL;
Widget *Widget::keyboardFocus=NULL;

Widget::Widget() : x(0),y(0),w(0),h(0),tip(NULL),mouseIn(false),mouseL(false),visible(true) {
	widgets.push(this);
}

Widget::Widget(int x,int y) : x(x),y(y),w(0),h(0),tip(NULL),mouseIn(false),mouseL(false),visible(true) {
	widgets.push(this);
}

Widget::Widget(int x,int y, int w, int h) : x(x),y(y),w(w),h(h),tip(NULL),mouseIn(false),mouseL(false),visible(true) {
	widgets.push(this);
}

Widget::~Widget() {
	if ( tip ) free(tip);
	if ( focus == this ) focus=NULL;
	widgets.remove(this);
}

void Widget::setBackgroundColor( const Doryen::Color col, const Doryen::Color colFocus )
{
	back=col;
	backFocus=colFocus;
}

void Widget::setForegroundColor( const Doryen::Color col, const Doryen::Color colFocus )
{
	fore=col;
	foreFocus=colFocus;
}

void Widget::setConsole( Doryen::Console *console )
{
	con=console;
}

void Widget::update(const TCOD_key_t k) {
	bool curs=TCODMouse::isCursorVisible();
	if ( curs ) {
		if ( mouse.cx >= x && mouse.cx < x+w && mouse.cy >= y && mouse.cy < y+h ) {
			if ( ! mouseIn ) {
				mouseIn = true;
				onMouseIn();
			}
			if ( focus != this ) {
				focus=this;
			}
		} else {
			if ( mouseIn ) {
				mouseIn = false;
				onMouseOut();
			}
			mouseL=false;
			if ( this == focus ) {
				focus = NULL;
			}
		}
	}
	if ( mouseIn || (! curs && this == focus ) ) {
		if ( mouse.lbutton && ! mouseL ) {
			mouseL = true;
			onButtonPress();
		} else if (! mouse.lbutton && mouseL ) {
			onButtonRelease();
			keyboardFocus=NULL;
			if ( mouseL ) onButtonClick();
			mouseL=false;
		} else if ( mouse.lbutton_pressed ) {
			keyboardFocus=NULL;
			onButtonClick();
		}
	}
}

void Widget::updateWidgetsIntern(const TCOD_key_t k) {
	elapsed=TCODSystem::getLastFrameLength();
	for (Widget **w=widgets.begin(); w!= widgets.end(); w++) {
		if ( (*w)->isVisible() ) {
			(*w)->computeSize();
			(*w)->update(k);
		}
	}
}

void Widget::updateWidgets(const TCOD_key_t k,const TCOD_mouse_t pmouse) {
	mouse=pmouse;
	updateWidgetsIntern(k);
}

void Widget::renderWidgets() {
    if ( !con )
    { con = Doryen::Console::root; }
    for (Widget **w=widgets.begin(); w!= widgets.end(); w++) {
		if ((*w)->isVisible()) (*w)->render();
	}
}

void Widget::move(int x,int y) {
	this->x=x;
	this->y=y;
}

void Widget::setTip(const char *tip) {
	if ( this->tip ) free(this->tip);
	this->tip = TCOD_strdup(tip);
}


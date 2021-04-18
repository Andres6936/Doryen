// Joan Andrés (@Andres6936) Github.

#include "Doryen/Renderer/Sdl/EventSdl.hpp"

using namespace Doryen;


EventSdl::EventSdl(SDL_Event& _event) noexcept: event(_event)
{
	// Construct the reference to main listener event of SDL.
}

void EventSdl::getGenericEvent(Key& key) noexcept
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

	case SDLK_a:
		key.setKeyCode(KeyCode::A);
		break;
	case SDLK_b:
		key.setKeyCode(KeyCode::B);
		break;
	case SDLK_c:
		key.setKeyCode(KeyCode::C);
		break;
	case SDLK_d:
		key.setKeyCode(KeyCode::D);
		break;
	case SDLK_e:
		key.setKeyCode(KeyCode::E);
		break;
	case SDLK_f:
		key.setKeyCode(KeyCode::F);
		break;
	case SDLK_g:
		key.setKeyCode(KeyCode::G);
		break;
	case SDLK_h:
		key.setKeyCode(KeyCode::H);
		break;
	case SDLK_i:
		key.setKeyCode(KeyCode::I);
		break;
	case SDLK_j:
		key.setKeyCode(KeyCode::J);
		break;
	case SDLK_k:
		key.setKeyCode(KeyCode::K);
		break;
	case SDLK_l:
		key.setKeyCode(KeyCode::L);
		break;
	case SDLK_m:
		key.setKeyCode(KeyCode::M);
		break;
	case SDLK_n:
		key.setKeyCode(KeyCode::N);
		break;
	case SDLK_o:
		key.setKeyCode(KeyCode::O);
		break;
	case SDLK_p:
		key.setKeyCode(KeyCode::P);
		break;
	case SDLK_q:
		key.setKeyCode(KeyCode::Q);
		break;
	case SDLK_r:
		key.setKeyCode(KeyCode::R);
		break;
	case SDLK_s:
		key.setKeyCode(KeyCode::S);
		break;
	case SDLK_t:
		key.setKeyCode(KeyCode::T);
		break;
	case SDLK_w:
		key.setKeyCode(KeyCode::W);
		break;
	case SDLK_x:
		key.setKeyCode(KeyCode::X);
		break;
	case SDLK_y:
		key.setKeyCode(KeyCode::Y);
		break;
	case SDLK_z:
		key.setKeyCode(KeyCode::Z);
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

	default:
		key.setKeyCode(KeyCode::NONE);
		break;
	}
}

// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_MOUSESDL_HPP
#define DORYEN_MOUSESDL_HPP

#include <Doryen/Event/Mouse.hpp>
#include <Doryen/Geometry/Size.hpp>
#include <Doryen/Renderer/Sdl/EventSdl.hpp>

namespace Doryen
{

	class MouseSdl
	{

	public:

		void updateGeneric(const EventSdl& event, Mouse& mouse) noexcept;

		void updateRelative(const Geometry::Size& sizeFont, Mouse& mouse) noexcept;

	};

}

#endif //DORYEN_MOUSESDL_HPP

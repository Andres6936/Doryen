// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_RENDER_HPP
#define DORYEN_RENDER_HPP

#include <any>

namespace Doryen
{

	class CallbackRender
	{

	public:

		virtual ~CallbackRender() = default;

		virtual void render(std::any _surface) = 0;

	};

}

#endif //DORYEN_RENDER_HPP

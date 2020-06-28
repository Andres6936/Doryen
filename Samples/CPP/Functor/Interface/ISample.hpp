// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_ISAMPLE_HPP
#define DORYEN_ISAMPLE_HPP

#include <string>

#include "Doryen/libtcod.hpp"

using namespace Doryen;

namespace Functor
{

	class ISample
	{

	private:

		const std::string name;

	protected:

		std::reference_wrapper<Console> sample;

	public:

		// Construct

		ISample(const std::string& _name, std::reference_wrapper<Console> _console) : name(_name), sample(_console)
		{
		};

		// Methods Virtual

		virtual void render(KeyCode key, const Mouse& mouse) = 0;

		// Getters

		const std::string& getName() const
		{
			return name;
		}

	};

}

#endif //DORYEN_ISAMPLE_HPP

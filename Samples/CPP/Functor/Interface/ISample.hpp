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

		/**
		 * To be able to use the same console throughout all the examples
		 * it is necessary to have a reference to it, this is important
		 * since the changes made in an example will be correctly rendered
		 * once it is drawn in the main method.
		 */
		Console& sample;

	public:

		// Construct

		ISample(const std::string& _name, Console& _console) : name(_name), sample(_console)
		{
			sample.setFramePerSeconds(30);
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

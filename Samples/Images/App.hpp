// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_APP_HPP
#define DORYEN_APP_HPP

#include <Doryen/Doryen.hpp>

using namespace Doryen;

namespace Sample
{

	class App
	{

	private:

		/**
		 * Default size of console 80x50.
		 */
		Console console{};

		/**
		 * Image of test, size of image 30x40.
		 */
		Image skull{ "Skull.png" };

	public:

		void render();

	};

}

#endif //DORYEN_APP_HPP

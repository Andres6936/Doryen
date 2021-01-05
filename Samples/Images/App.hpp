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

		// Properties

		/**
		 * Determine the scale coefficient in axis x and y of image, 1.0f is original size;
		 */
		float scaleImage = 1.0f;

		/**
		 * Default size of console 80x50.
		 */
		Console console{};

		/**
		 * Image of test, size of image 30x40.
		 */
		Image skull{ "Skull.png" };

		// Methods Private

		void handleInput(const KeyCode key);

	public:

		// Methods Public

		/**
		 * Render the console.
		 */
		void render();

	};

}

#endif //DORYEN_APP_HPP

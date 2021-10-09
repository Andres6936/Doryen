#ifndef LIBTCOD_SFML_HPP
#define LIBTCOD_SFML_HPP

#include <Doryen/Renderer/Renderer.hpp>
#include <SFML/Window.hpp>

namespace Doryen
{
    class SFML : public Renderer
    {

	private:

		sf::Window window;

    public:

		virtual ~SFML();

		void onRenderer() override;

        void draw() override;

        void loadFont() override;

        void showCursor(bool visible) override;

        void setWindowTitle(const std::string &_title) override;

        void registerCallback(std::unique_ptr<CallbackRender> _callback) override;
    };
}


#endif //LIBTCOD_SFML_HPP

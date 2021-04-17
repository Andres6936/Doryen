// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_DUMMY_HPP
#define DORYEN_DUMMY_HPP

#include <Doryen/Renderer/Renderer.hpp>

namespace Doryen
{

	class Dummy : public Renderer
	{

	public:

		void onRenderer() override;

		void draw() override;

		void loadFont() override;

		void showCursor(bool visible) override;

		void setWindowTitle(const std::string& _title) override;

		void registerCallback(std::unique_ptr<CallbackRender> _callback) override;

	};

}

#endif //DORYEN_DUMMY_HPP

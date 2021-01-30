// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_ACCESSPRIVATE_HPP
#define DORYEN_ACCESSPRIVATE_HPP

#include <Doryen/Doryen.hpp>

namespace Doryen::Test
{
	namespace
	{
		struct TranslationUnitTag {};
	}

	template<typename UniqueTag, auto MemberPointer>
	class AccessPrivateMemberConsole
	{

	public:

		friend std::vector<Char>& accessPrivateMainBuffer(Console& console)
		{
			return console.*MemberPointer;
		}

	};

	std::vector<Char>& accessPrivateMainBuffer(Console& console);

	template struct AccessPrivateMemberConsole<TranslationUnitTag, &Console::buffer>;

}

#endif //DORYEN_ACCESSPRIVATE_HPP

// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_ACCESSPRIVATE_HPP
#define DORYEN_ACCESSPRIVATE_HPP

#include <Doryen/Doryen.hpp>

// The follow code is based in: https://wandbox.org/permlink/ttKuVlrsMe2AycJo


namespace Doryen::Test
{
	// Unique namespace in each TU.
	namespace
	{
		// Unique type in each TU (internal linkage).
		struct TranslationUnitTag {};
	}

	// 'Console::<any private member>' accessor.
	template<typename UniqueTag, auto MemberPointer>
	class AccessPrivateMemberConsole
	{

	public:

		// (Injected) friend definition.
		friend std::vector<Char>& accessPrivateMainBuffer(Console& console)
		{
			return console.*MemberPointer;
		}

	};

	// Friend (re-)declaration.
	std::vector<Char>& accessPrivateMainBuffer(Console& console);

	// Single explicit instantiation definition.
	template struct AccessPrivateMemberConsole<TranslationUnitTag, &Console::buffer>;

}

#endif //DORYEN_ACCESSPRIVATE_HPP

// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_ACCESSPRIVATE_HPP
#define DORYEN_ACCESSPRIVATE_HPP

#include <Doryen/Doryen.hpp>

// The explications for this mechanism is in: https://dfrib.github.io/a-foliage-of-folly/
// The follow code is based in: https://wandbox.org/permlink/ttKuVlrsMe2AycJo
// Please, see this repository: https://github.com/dfrib/accessprivate


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
	class AccessPrivateBufferConsole
	{

	public:

		// (Injected) friend definition.
		friend std::vector<Char>& accessPrivateBuffer(Console& console)
		{
			return console.*MemberPointer;
		}

	};

	// Friend (re-)declaration.
	std::vector<Char>& accessPrivateBuffer(Console& console);

	// Single explicit instantiation definition.
	template struct AccessPrivateBufferConsole<TranslationUnitTag, &Console::buffer>;

}

#endif //DORYEN_ACCESSPRIVATE_HPP

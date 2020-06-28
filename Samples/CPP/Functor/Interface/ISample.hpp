// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_ISAMPLE_HPP
#define DORYEN_ISAMPLE_HPP

#include <string>

namespace Functor
{

	class ISample
	{

	private:

		const std::string name;

	public:

		ISample(const std::string& _name) : name(_name)
		{
		};

	};

}

#endif //DORYEN_ISAMPLE_HPP

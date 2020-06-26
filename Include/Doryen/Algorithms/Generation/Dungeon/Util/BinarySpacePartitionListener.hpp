#ifndef LIBTCOD_BINARYSPACEPARTITIONLISTENER_HPP
#define LIBTCOD_BINARYSPACEPARTITIONLISTENER_HPP

namespace Doryen
{
	namespace Algorithms
	{

		class BinarySpacePartition;

		class ITCODBspCallback
		{

		public :

			virtual ~ITCODBspCallback()
			{
			}

			virtual bool visitNode(BinarySpacePartition* node, void* userData) = 0;
		};
	}
}

#endif //LIBTCOD_BINARYSPACEPARTITIONLISTENER_HPP

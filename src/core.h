#pragma once
#include "boost/any.hpp"
class coreController
{
	public:
		bool impassables[7][7];
		bool (*walls)[5];
		//unsigned int (*solids)[5];
		boost::any solids[5][5];
		coreController();
};

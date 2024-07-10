#pragma once
#include "boost/any.hpp"
#include <vector>
#include <array>
class coreController
{
	public:
		coreController(std::string);
		int levelSize = 7;
		std::vector<std::vector<bool>> impassables;
		std::vector<std::vector<boost::any>> solids;
		std::vector<std::vector<bool>> walls;
		std::vector<std::vector<bool>> loadLevelWalls();
		
		std::vector<std::vector<unsigned int>> loadLevelSolids();
		std::string dir;
};

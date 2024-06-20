#include "core.h"
#include "config.h"
coreController::coreController()
{
	std::cout << "\n\n\n" << "creating array of walls" << "\n";
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			if(i < 1 or i >= 6 or j < 1 or j >= 6)
			{
				impassables[i][j] = true;
			}

			std::cout << impassables[i][j] << " ";
		}
		std::cout << "\n";
	}
}

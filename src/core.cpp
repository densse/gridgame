#include "core.h"
#include "config.h"
#include <array>
coreController::coreController(std::string directory)
{
	dir = directory;
	for(int i=0; i<levelSize+2; i++)
	{
		std::vector <bool> row;
		for(int j=0; j<levelSize+2; j++)
		{
			row.push_back(0);
		}
		impassables.push_back(row);
	}

	for(int i=0; i<levelSize+2; i++)
	{
		std::vector <boost::any> row;
		for(int j=0; j<levelSize+2; j++)
		{
			row.push_back(0);
		}
		solids.push_back(row);
	}

	std::cout << "\n\n\n" << "creating array of walls" << "\n";
	for(int i = 0; i < levelSize+2; i++)
	{
		for(int j = 0; j < levelSize+2; j++)
		{
			if(i < 1 or i >= levelSize+1 or j < 1 or j >= levelSize+1)
			{
				impassables[i][j] = true;
			}
			else
			{
				impassables[i][j] = false;
			}

			std::cout << impassables[i][j] << " ";
		}
		std::cout << "\n";
	}

}

std::vector<std::vector<bool>> coreController::loadLevelWalls()
{
	std::cout << "\n\nloading level walls" << "\n";
	std::vector<std::vector<bool>> w;

	for(int i=0; i<levelSize; i++)
	{
		std::vector <bool> row;
		for(int j=0; j<levelSize; j++)
		{
			row.push_back(0);
		}
		w.push_back(row);
	}

	std::ifstream data (dir + "/walls.w");
	std::string d;
	if(data.is_open())
	{
		for(int y = levelSize-1; y >= 0; y--)
		{
			for(int x = 0; x < levelSize; x++)
			{
				d = data.get();
				if(d == "1")
				{
					w[x][y] = 1;
					std::cout << w[x][y];
				}
				else if(d == "0")
				{
					w[x][y] = 0;
					std::cout << w[x][y];
				}
				else if(d == "\n")
				{
					std::cout << "newline char skipping\n";
					x--;
				}
				else
				{
					std::cout << "undefined symbol: " << d << " continuing" << "\n";
					x--;
				}
			}
			std::cout << "\n";
		}
	}
	else
	{
		std::cout << "file not opened properly" << std::endl;
	}
	data.close();
	std::cout << "complete\n";
	return w;

}


std::vector<std::vector<unsigned int>> coreController::loadLevelSolids()
{
	std::vector<std::vector<unsigned int>> s;
	for(int i=0; i<levelSize; i++)
	{
		std::vector <unsigned int> row;
		for(int j=0; j<levelSize; j++)
		{
			row.push_back(0);
		}
		s.push_back(row);
	}
	std::cout << "\n\nloading level solids" << "\n";
	
	std::ifstream data (dir + "/solids.s");
	std::string d;
	if(data.is_open())
	{
		for(int y = levelSize-1; y >= 0; y--)
		{
			for(int x = 0; x < levelSize; x++)
			{
				d = data.get();
				if(d == "\n")
				{
					std::cout << "newline char skipping" << "\n";
					x--;
				}
				else
				{
					s[x][y] = std::stoi(d);
					std::cout << s[x][y];
				}
			}
			std::cout << "\n";
		}
	}
	else
	{
		std::cout << "file not opened properly" << std::endl;
	}
	data.close();
	return s;
}

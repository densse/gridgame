#include "config.h"

std::string loadString(const std::string& filename)
{
	std::string result = "";

	std::string line = "";
	std::ifstream myFile(filename.c_str());

	if(myFile.is_open())
	{
		while(std::getline(myFile, line))
		{
			result += line + "\n";
		}
		myFile.close();
	}
	std::cout << "loaded shader\n";
	return result;
}

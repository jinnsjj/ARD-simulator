#include "tools.h"
#include <iostream>


Tools::Tools()
{
}


Tools::~Tools()
{
}

//template <typename T>
void Tools::Visualize2dVector(std::vector<std::vector<int>> vv)
{
	std::cout << "<" << std::to_string(vv.size()) << "," << std::to_string(vv[0].size()) << ">" << std::endl;
	for (auto v : vv)
	{
		for (auto i : v)
		{
			std::cout << std::to_string(i) << " ";
		}
		std::cout << std::endl;
	}
}

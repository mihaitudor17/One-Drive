#include "json.hpp"
#include <iostream>
#include <fstream>
#include "JSON.h"

int main()
{
	JSON j;

	std::cout << j.verifyFileExistance("heroes.json");

	return 0;
}
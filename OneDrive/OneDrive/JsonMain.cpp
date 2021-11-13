#include "json.hpp"
#include <iostream>
#include <fstream>
#include "JSON.h"

int main()
{
	JSON j("./heroes.json", "heroes.json");

	j.readJsonFile();

	return 0;
}
#include "json.hpp"
#include <iostream>
#include <fstream>
#include "JSON.h"

int main()
{
	JSON j("./heroes.json", "heroes.json");

	j.readJsonFile();
	j.inputJsonAndOutputInAnotherJson();

	return 0;
}
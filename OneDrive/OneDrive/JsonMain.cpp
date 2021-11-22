#include "json.hpp"
#include <iostream>
#include <fstream>
#include "JSON.h"

int main()
{
	JSON j("./userOutput.json", "userOutput.json");

	j.writeUserInfoToJsonFile("Catalin");
	j.writeUserInfoToJsonFile("Sorin");
	j.writeUserInfoToJsonFile("Mihai");
	j.writeUserInfoToJsonFile("Tinel");

	j.writeUsersToFile();

	return 0;
}
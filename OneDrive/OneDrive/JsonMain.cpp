#include "json.hpp"
#include <iostream>
#include <fstream>
#include "JSON.h"

int main()
{
	JSON j(". / userOutput.json", "userOutput.json");

	j.writeUsersWithoutOverwriting();

	return 0;
}
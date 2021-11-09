#include <iostream>
#include <fstream>
#include "json.hpp"

int main()
{
	nlohmann::json readData;
	std::string fileName = "heroes";

	std::ifstream in(fileName + ".json");
	in >> readData;

	std::cout << readData["squadName"] << '\n';				//returns the content of "squadName" from .json file
	std::cout << readData["active"] << '\n';				//returns a boolean value from the .json file
	std::cout << readData["members"][0]["powers"] << '\n';	//returns the superpowers of the member from the first position in the members array from .json file

	std::cout << "\n\n";

	//std::cout << readData["members"]["powers"] << '\n';	//ERROR: the command doesn't know which hero member we are reffering to
	//std::cout << readData["formd"] << '\n';				//this command returns null, because we mispelled a component's name from the .json file



	std::cout << "\n\n";

	std::cout << readData["members"][0]["powers"][1] << '\n';//shows the second power of the first member from the hero squad
	std::cout << readData["members"][0]["powers"][5] << '\n';//shows null, because power number 6 does not exist

	std::cout << "\n\n";

	for (int index = 0; index < 3; index++)
	{
		std::cout << readData["members"][index]["secretIdentity"] << '\n';
	}//this command returns all members' secret identities, because of the index

	std::cout << "\n\n";

	for (int index = 0; index < 3; index++)
	{
		std::cout << readData["members"][index] << '\n';
	}//shows all members information, including the fields names, in lexical order of the fields



	return 0;
}
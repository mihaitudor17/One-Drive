#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>

int main()
{
	//initialise folder path
	auto path = std::filesystem::temp_directory_path() / "C:/Users/gliza/Desktop/Synchronized Folder";

	for (auto& p : std::filesystem::directory_iterator(path)) {
		std::string fileName = p.path().string();
		std::cout << fileName << std::endl;
	}
}
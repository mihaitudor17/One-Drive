#pragma once
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
class Folder
{
public:
	std::string m_folderName;
	int m_numberOfFiles;
	std::filesystem::path m_path;
	long long m_lastWrittenTime;
public:
	Folder(const std::string &name,std::filesystem::path path);
	void assignLastWrittenTime();
	void assignNumberOfFiles();
};


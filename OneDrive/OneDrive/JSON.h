#pragma once
#include "json.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

class JSON
{
private:
	std::string m_path;
	std::string m_fileName;
	nlohmann::json m_j;

public:
	JSON();
	JSON(std::string path, std::string fileName);

	void setPath(std::string path);
	void setFileName(std::string fileName);

	std::string getPath()const;
	std::string getFileName()const;

	bool verifyFileExistance(const std::string& fileName);
	void createUser(std::string user);
	void writeUserInfoToJsonFile(const std::string& fileName);
	void readJsonFile();
	void inputJsonAndOutputInAnotherJson();
	void writeUsersWithoutOverwriting(const std::string & text);
	void to_json(nlohmann::json& j, const std::string& text);

};
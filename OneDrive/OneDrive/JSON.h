#pragma once
#include "json.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <unordered_set>

class JSON
{
private:
	std::string m_path;
	std::string m_fileName;
	nlohmann::json m_body;
	std::vector<nlohmann::json> m_userNames;
	std::unordered_set<std::string> m_uniqueUserNames;

public:
	JSON();
	JSON(const std::string& path, const std::string& fileName);

	void setPath(const std::string& path);
	void setFileName(const std::string& fileName);
	void setBody(nlohmann::json body);
	void setUserNames( std::vector<nlohmann::json>& userNames);
	void setUniqueUsernames(const std::unordered_set<std::string>& uniqueUserNames);

	std::string getPath()const;
	std::string getFileName()const;
	nlohmann::json getBody()const;
	std::vector<nlohmann::json> getUserNames()const;
	std::unordered_set<std::string> getUniqueUserNames()const;

	bool verifyFileExistance(const std::string& fileName);
	void createUser(std::string user);
	void inputJsonAndOutputInAnotherJson();
	void addNewUser(const std::string& userName);
	void writeUsersWithoutOverwriting(const std::string& text);
	void writeUsersToFile();
	void inputJson(const std::string& fileName);

};
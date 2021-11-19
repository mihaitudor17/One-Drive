#include "JSON.h"

JSON::JSON()
{
	/*empty*/
}

JSON::JSON(std::string path, std::string fileName)
{
	m_path = path;
	m_fileName = fileName;
}

void JSON::setPath(std::string path)
{
	m_path = path;
}

void JSON::setFileName(std::string fileName)
{
	m_fileName = fileName;
}

std::string JSON::getPath() const
{
	return m_path;
}

std::string JSON::getFileName() const
{
	return m_fileName;
}

bool JSON::verifyFileExistance(const std::string& fileName)
{
	std::ifstream const file(fileName);
	if (!file)
		return false;
	else
		return true;
}

void JSON::createUser(std::string user)
{
	nlohmann::json userJson;

	userJson = user;

	std::ofstream jsonFile("out.json");
	jsonFile << std::setw(1) << userJson;
}

void JSON::writeUserInfoToJsonFile(std::string& fileName)
{

}

void JSON::readJsonFile()
{
	nlohmann::json readData;

	std::ifstream in(m_fileName);
	in >> readData;
}

void JSON::inputJsonAndOutputInAnotherJson()
{
	nlohmann::json inputJson;

	std::ifstream input("heroes.json", std::ifstream::binary);
	input >> inputJson;

	std::ofstream output("out.json");
	output << std::setw(1) << inputJson;
}

void JSON::writeUsersWithoutOverwriting(const std::string & text)
{
	std::ofstream out(getFileName(), std::fstream::app);


	out << text << '\n';

	out.close();
}

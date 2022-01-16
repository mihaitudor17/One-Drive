#include "JSON.h"

JSON::JSON()
{
	/*empty*/
}

JSON::JSON(const std::string& path, const std::string& fileName) : m_path{ path }, m_fileName{ fileName }
{
	m_body["user"] = m_userNames;
}

void JSON::setPath(const std::string& path)
{
	m_path = path;
}

void JSON::setFileName(const std::string& fileName)
{
	m_fileName = fileName;
}

void JSON::setBody(nlohmann::json body)
{
	m_body = body;
}

void JSON::setUserNames(std::vector<nlohmann::json>& userNames)
{
	m_userNames = userNames;
}

void JSON::setUniqueUsernames(const std::unordered_set<std::string>& uniqueUserNames)
{
	m_uniqueUserNames = uniqueUserNames;
}

std::string JSON::getPath() const
{
	return m_path;
}

std::string JSON::getFileName() const
{
	return m_fileName;
}

nlohmann::json JSON::getBody() const
{
	return m_body;
}

std::vector<nlohmann::json> JSON::getUserNames() const
{
	return m_userNames;
}

std::unordered_set<std::string> JSON::getUniqueUserNames() const
{
	return m_uniqueUserNames;
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

void JSON::addNewUser(const std::string& userName)
{
	nlohmann::json aux;
	aux["username"] = userName;
	m_userNames.push_back(aux);
	
	m_body["user"]=m_userNames;
}


void JSON::inputJsonAndOutputInAnotherJson()
{
	nlohmann::json inputJson;

	std::ifstream input("heroes.json", std::ifstream::binary);
	input >> inputJson;

	std::ofstream output("out.json");
	output << std::setw(1) << inputJson;
}

void JSON::writeUsersWithoutOverwriting(const std::string& text)
{
	std::ofstream out(getFileName(), std::fstream::app);

	out << text << '\n';

	out.close();
}

void JSON::writeUsersToFile()
{
	std::ofstream out(m_fileName);
	
	out << std::setw(2) << m_body;

	out.close();
}

void JSON::inputJson(const std::string& fileName)
{
	nlohmann::json inputJson;

	std::ifstream input(fileName, std::ifstream::binary);
	input >> inputJson;

	m_body=inputJson;
}

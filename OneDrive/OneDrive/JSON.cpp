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

void JSON::writeUserInfoToJsonFile(const std::string& fileName)
{
	std::vector<nlohmann::json> v;

	nlohmann::json aux1;
	aux1["username"] = "Catalin";
	v.push_back(aux1);

	nlohmann::json aux2;
	aux2["username"] = "Tinel";
	v.push_back(aux2);

	m_j["user"] = v;
	std::ofstream out(fileName);
	out << std::setw(2) << m_j;
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

void JSON::writeUsersWithoutOverwriting(const std::string& text)
{
	std::ofstream out(getFileName(), std::fstream::app);


	out << text << '\n';

	out.close();
}

void JSON::to_json(nlohmann::json& j, const std::string& text)
{
	j = { {"username", text} };
}

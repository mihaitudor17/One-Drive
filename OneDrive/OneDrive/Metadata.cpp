#include "Metadata.h"

Metadata::Metadata()
{
}

void Metadata::setPath(const std::string& path)
{
	m_path = path;
}

Metadata::Metadata(const std::string& path):m_path{path}
{

}

void Metadata::setBody(const nlohmann::json& body)
{
	m_body = body;
}

std::filesystem::path Metadata::getPath() const
{
	return m_path;
}

nlohmann::json Metadata::getBody() const
{
	return m_body;
}
void Metadata::inputJson(const std::string& path)
{
	nlohmann::json inputJson;

	std::ifstream input(path, std::ifstream::binary);
	input >> inputJson;

	m_body = inputJson;
}

void Metadata::outputJson(const std::string& path)
{
	std::ofstream output(path);
	output << std::setw(1) << m_body;
}


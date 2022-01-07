#include "Metadata.h"

Metadata::Metadata()
{
}

void Metadata::setPath(const std::filesystem::path& path)
{
	m_path = path;
}

Metadata::Metadata(const std::filesystem::path& path):m_path{path}
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

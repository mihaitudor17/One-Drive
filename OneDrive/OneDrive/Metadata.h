#pragma once
#include "json.hpp"
#include <filesystem>
#include <string>
#include <fstream>
#include "FowlerNollVo.h"
class Metadata
{
private:
	std::filesystem::path m_path;
	nlohmann::json m_body;

public:
	std::vector<nlohmann::json> m_files;
	Metadata();
	Metadata(const std::string& path);

	void setBody(const nlohmann::json& body);
	void setPath(const std::string& path);

	std::filesystem::path getPath()const;
	nlohmann::json getBody()const;

	void inputJson(const std::string& path);
	void outputJson(const std::string& path);
	void folderMetadata(const std::string& path);
};


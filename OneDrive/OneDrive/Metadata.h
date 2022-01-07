#pragma once
#include "json.hpp"
#include <filesystem>
#include <string>
class Metadata
{
private:
	std::filesystem::path m_path;
	nlohmann::json m_body;

	Metadata();
	Metadata(const std::filesystem::path& path);
};


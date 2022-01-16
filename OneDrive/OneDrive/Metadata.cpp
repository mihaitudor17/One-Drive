#include "Metadata.h"

Metadata::Metadata()
{
}

void Metadata::setPath(const std::string& path)
{
	m_path = path;
}

Metadata::Metadata(const std::string& path) :m_path{ path }
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

void Metadata::folderMetadata(const std::string& path)
{
	FowlerNollVo hashFunction;
	for (auto p : std::filesystem::directory_iterator(path)) {
		if (p.is_directory()) {
			nlohmann::json data;
			data["hash"] = hashFunction.getHashOfFolder(p.path().string());
			if (data["hash"] == 0) {
				data["hash"] = hashFunction.hashByName(p.path().string());
			}
			long long lastWriteTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			data["lastWriteTime"] = lastWriteTime;
			m_body[p.path().filename().string()] = data;
		}
		else if (p.path().filename().string().find(".mp4") != std::string::npos ||
			p.path().filename().string().find(".jpg") != std::string::npos ||
			p.path().filename().string().find(".png") != std::string::npos
			) {
			nlohmann::json data;
			data["hash"] = hashFunction.hashingImageFileAndVideoFile(p.path().string());
			long long lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			data["lastWriteTime"] = lastWrittenTime;
			m_body[p.path().filename().string()] = data;
		}
		else if (p.path().filename().string().find(".txt") != std::string::npos) {
			nlohmann::json data;
			data["hash"] = hashFunction.hashingTextFile(p.path().string());
			long long lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			data["lastWriteTime"] = lastWrittenTime;
			m_body[p.path().filename().string()] = data;
		}
		else {
			nlohmann::json data;
			data["hash"] = 0;
			long long lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			data["lastWriteTime"] = lastWrittenTime;
			m_body[p.path().filename().string()] = data;
		}
	}
}
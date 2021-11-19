#include "Folder.h"

Folder::Folder(const std::string& name, std::filesystem::path path):
	m_folderName{ name }, m_path{ path }
{
}

void Folder::assignLastWrittenTime()
{
	m_lastWrittenTime = 0;
	for (auto& p : std::filesystem::directory_iterator(m_path)) {
		std::string fileName = p.path().string();
		if (m_lastWrittenTime < std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count()) {
			m_lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
		}
	}
}

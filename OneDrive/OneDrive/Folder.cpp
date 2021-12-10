#include "Folder.h"
void copyDirectoryContentsFromTo(std::filesystem::path source, std::filesystem::path destination) {
	for (auto& it : std::filesystem::directory_iterator(source)) {
		if (std::filesystem::exists(destination / it.path().filename())) {
			remove(destination / it.path().filename());
		}
		std::filesystem::copy(it, destination);
	}
}

Folder::Folder(const std::string& name, std::filesystem::path path) :
	m_folderName{ name }, m_path{ path }
{
	m_lastWrittenTime = 0;
	m_numberOfFiles = 0;
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

void Folder::assignNumberOfFiles()
{
	for (auto& p : std::filesystem::directory_iterator(m_path)) {
		m_numberOfFiles++;
	}
}

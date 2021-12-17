#include "Folder.h"
void copyDirectoryContents(std::filesystem::path source, std::filesystem::path destination) {
	for (auto it : std::filesystem::directory_iterator(source)) {
		if (it.is_directory()) {
			std::filesystem::create_directory(destination / it.path().filename());
			copyDirectoryContents(it.path(), destination / it.path().filename());
		}
		else {
			if (std::filesystem::exists(destination / it.path().filename())) {
				remove(destination / it.path().filename());
			}
			std::filesystem::copy(it.path(), destination);
		}
	}
}

Folder::Folder(const std::string& name, std::filesystem::path path) :
	m_folderName{ name }, m_path{ path }
{
	m_lastWrittenTime = 0;
	m_numberOfFiles = 0;
}

void Folder::assignLastWrittenTime(const std::filesystem::path& m_path)
{
	std::cout << "Sunt in " << m_path.filename().string() << std::endl;
	for (auto p : std::filesystem::directory_iterator(m_path)) {
		std::string fileName = p.path().string();
		std::cout << fileName << std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count() << std::endl;
		if (p.is_directory()) {
			this->assignLastWrittenTime(p.path());
		}
		else {
			if (m_lastWrittenTime < std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count()) {
				m_lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			}
		}
	}
}

void Folder::assignNumberOfFiles()
{
	for (auto& p : std::filesystem::directory_iterator(m_path)) {
		m_numberOfFiles++;
	}
}

#include "Window.h"
#include "TCP_Client.h"
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QString>
#include "Folder.h"
#include <thread>
#include "Metadata.h"
void keepFoldersInSync(Folder& folder1, Folder& folder2) {
	while (true) {
		folder1.assignLastWrittenTime(folder1.m_path);
		folder2.assignLastWrittenTime(folder2.m_path);
		if (folder1.m_lastWrittenTime > folder2.m_lastWrittenTime) {
			copyDirectoryContents(folder1.m_path, folder2.m_path);
		}
		else if (folder2.m_lastWrittenTime > folder1.m_lastWrittenTime) {
			copyDirectoryContents(folder2.m_path, folder1.m_path);
		}
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}


void syncFolderWithMetadata(const std::filesystem::path& path, const Metadata& metadata) {
	FowlerNollVo hashFunction;
	std::unordered_set<long long> hashes;
	for (const auto& it : std::filesystem::directory_iterator(path)) {
		//daca exista fisierul
		if (metadata.m_body.find(it.path().string()) != metadata.m_body.end()) {
			long long lastWriteTime = std::chrono::duration_cast<std::chrono::milliseconds>(it.last_write_time().time_since_epoch()).count();
			if (lastWriteTime > metadata.m_body[it.path().string()]["lastWriteTime"]) {
				std::cout << it.path().string() << ": trebuie actualizat" << std::endl;
			}
		}
		else {//cazul de redenumire
			int rename = 0;
			std::string renamed;
			for (const auto& item : metadata.m_body.items()) {
				if (std::filesystem::is_directory(it.path())) {
					size_t hash = hashFunction.getHashOfFolder(it.path().string());
					if (item.value()["hash"] == hash) {
						rename = 1;
						hashes.insert(hash);
						renamed = item.key();
						break;
					}
				}
				else if (it.path().filename().string().find(".txt") != std::string::npos)
				{
					size_t hash = hashFunction.hashingTextFile(it.path().string());
					if (item.value()["hash"] == hash) {
						rename = 1;
						hashes.insert(hash);
						renamed = item.key();
						break;
					}

				}
				else
					if (it.path().filename().string().find(".mp4") != std::string::npos ||
						it.path().filename().string().find(".jpg") != std::string::npos ||
						it.path().filename().string().find(".png") != std::string::npos
						) {
						size_t hash = hashFunction.hashingImageFileAndVideoFile(it.path().string());
						if (item.value()["hash"] == hash) {
							rename = 1;
							hashes.insert(hash);
							renamed = item.key();
							break;
						}
					}

			}
			if (rename) {
				std::cout << renamed << " fisier redenumit in: " << it.path().string() << std::endl;
			}
			else {//nu exista pana acum
				std::cout << "fisier/folder nou: " << it.path().string() << std::endl;

			}
		}
	}
	//cazul de stergere
	for (const auto& item : metadata.m_body.items()) {
		int name = 1, hash = 1;
		if (!std::filesystem::exists(item.key())) {
			name = 0;
		}
		if (hashes.find(item.value()["hash"]) == hashes.end()) {
			hash = 0;
		}
		if (name == 0 && hash == 0) {
			std::cout << item.key() << " a fost sters" << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Window w;
	Client client;
	w.show();
	//initialise folder path
	/*Folder folder1("Synchronized Folder 1", "./Synchronized Folder");
	Folder folder2("Synchronized Folder 2", "../TCP_Server/Synchronized Folder");
	folder1.assignNumberOfFiles();
	folder2.assignNumberOfFiles();*/
	//keepFoldersInSync(folder1, folder2);

	Metadata metadata;
	metadata.inputJson("./sincronizare.json");
	syncFolderWithMetadata("./StoredFiles/tinel", metadata);

	return a.exec();
}

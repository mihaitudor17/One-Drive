#include "Window.h"
#include "TCP_Client.h"
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QString>
#include "Folder.h"
#include <thread>
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
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Window w;
	Client client;
	char* username = "Tinel";//De obtinut userul
	std::string path = "./Stored Files";
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	path += "/";
	path += username;
	client.connectServer();
	client.sendUser(client.getSock(), username);
	char fileName[FILENAME_MAX];
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	int ok;
	do {
		char fileName[FILENAME_MAX];
		ok = 0;
		char fileType[FILENAME_MAX];
		strcpy(fileName, client.recvFileName(client.getSock()));
		strcpy(fileType, client.recvFileName(client.getSock()));
		std::cout << fileName << std::endl << fileType << std::endl;
		if (strstr(fileType, "folder"))
		{
			std::filesystem::create_directory(fileName);
		}
		else
		{
			long size = client.recvFileSize(client.getSock());
			if (size != 0)
			{

				if (client.writeToFile(client.getSock(), fileName, size) == 0)
					ok = 1;
			}
			else
			{
				std::ofstream file;
				file.open(fileName);
			}
		}
	} while (ok!=1);
	w.show();
	//initialise folder path
	/*Folder folder1("Synchronized Folder 1", "./Synchronized Folder");
	Folder folder2("Synchronized Folder 2", "../TCP_Server/Synchronized Folder");
	folder1.assignNumberOfFiles();
	folder2.assignNumberOfFiles();*/
	//keepFoldersInSync(folder1, folder2);
	return a.exec();
}

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
		std::cout << folder1.m_lastWrittenTime << ' ' << folder2.m_lastWrittenTime << ' ' << "The last modified folder is: ";
		if (folder1.m_lastWrittenTime > folder2.m_lastWrittenTime) {
			std::cout << folder1.m_folderName;
			copyDirectoryContents(folder1.m_path, folder2.m_path);
		}
		else if (folder2.m_lastWrittenTime > folder1.m_lastWrittenTime) {
			std::cout << folder2.m_folderName;
			copyDirectoryContents(folder2.m_path, folder1.m_path);
		}
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
}
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Window w;

	w.show();
	if (!std::filesystem::exists("./Synchronized Folder 1")) {
		std::filesystem::create_directory("./Synchronized Folder 1");
	}
	if (!std::filesystem::exists("./Synchronized Folder 2")) {
		std::filesystem::create_directory("./Synchronized Folder 2");
	}
	//initialise folder path
	Folder folder1("Synchronized Folder 1", "./Synchronized Folder 1");
	Folder folder2("Synchronized Folder 2", "./Synchronized Folder 2");
	folder1.assignNumberOfFiles();
	folder2.assignNumberOfFiles();
	keepFoldersInSync(folder1, folder2);
	return a.exec();
}

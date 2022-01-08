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
	Metadata metadata("/metadata.json");
	return a.exec();
}

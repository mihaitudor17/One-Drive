#include "Window.h"
#include "TCP_Client.h"
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QString>
#include "Folder.h"
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
	folder1.assignLastWrittenTime();
	folder1.assignNumberOfFiles();
	folder2.assignLastWrittenTime();
	folder2.assignNumberOfFiles();
	int i = 0;
	return a.exec();
	while (true) {
		std::cout << "Iteration " << ++i;
		//std::cout << "The last modified folder is: ";
		//if (folder1.m_lastWrittenTime > folder2.m_lastWrittenTime) {
		//	std::cout << folder1.m_folderName;
		//}
		//else {
		//	std::cout << folder2.m_folderName;
		//}
		//client();
		//copyDirectoryContents("./Synchronized Folder 1", "./Synchronized Folder 2");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return a.exec();
}

#include "Window.h"
#include "TCP_Client.h"
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QString>
#include "Folder.h">
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.setFixedSize(700, 700);
    w.show();
	//initialise folder path
	std::string folder1 = "Synchronized Folder 1";
	std::string folder2 = "Synchronized Folder 2";
	int folder1NumberOfFiles = 0;
	int folder2NumberOfFiles = 0;
	std::filesystem::path path1 = "./"+ folder1;
	std::filesystem::path path2 = "./"+ folder2;
	std::ofstream f("Output.txt");
	long long lastWrittenTime=0;
	std::string lastWrittenFolder;

	Folder test("Synchronized Folder 1",path1);
	test.assignLastWrittenTime();
	std::cout << test.m_lastWrittenTime << std::endl;
	if (!std::filesystem::exists(path1)) {
		std::filesystem::create_directory(path1);
	}
	if (!std::filesystem::exists(path2)) {
		std::filesystem::create_directory(path2);
	}

	for (auto& p : std::filesystem::directory_iterator(path1)) {
		std::string fileName = p.path().string();
		if (lastWrittenTime < std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count()) {
			lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			lastWrittenFolder = folder1;
		}
		folder1NumberOfFiles++;
	}
	std::cout << lastWrittenTime << std::endl;
	for (auto& p : std::filesystem::directory_iterator(path2)) {
		std::string fileName = p.path().string();
		if (lastWrittenTime < std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count()) {
			lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			lastWrittenFolder = folder2;
		}
		folder2NumberOfFiles++;
	}
	std::cout << "Copying everything from folder 1 into folder 2:\n";
	for (auto& it : std::filesystem::directory_iterator(path1)) {
		std::cout << it.path().string() << std::endl;
		if (std::filesystem::exists(path2 / it.path().filename())) {
			remove(path2 / it.path().filename());
		}
		std::filesystem::copy(it, path2);

	}//trimit getline din it in path2 pe server
	f << "The last modified folder is: " << lastWrittenFolder;
	//client(argc, argv);
    return a.exec();
}

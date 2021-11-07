#include "Window.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <QWidget>
#include <QObject>
#include <QString>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.setFixedSize(700, 700);
    w.show();
	//initialise folder path
	std::string folder1 = "Synchronized Folder 1";
	std::string folder2 = "Synchronized Folder 2";
	std::string path1 = "./"+ folder1;
	std::string path2 = "./"+ folder2;
	std::ofstream f("Output.txt");
	long long lastWrittenTime=0;
	std::string lastWrittenFolder;
	for (auto& p : std::filesystem::directory_iterator(path1)) {
		std::string fileName = p.path().string();
		if (lastWrittenTime < std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count()) {
			lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			lastWrittenFolder = folder1;
		}
	}
	for (auto& p : std::filesystem::directory_iterator(path2)) {
		std::string fileName = p.path().string();
		if (lastWrittenTime < std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count()) {
			lastWrittenTime = std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count();
			lastWrittenFolder = folder2;
		}
	}
	f << "The last modified folder is: " << lastWrittenFolder;
    return a.exec();
}

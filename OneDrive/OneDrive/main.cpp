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
	auto path = "C:/Users/gliza/Desktop/Synchronized Folder 1";
	auto path2 = "C:/Users/gliza/Desktop/Synchronized Folder 2";

	std::cout << "Folder 1:\n";
	for (auto& p : std::filesystem::directory_iterator(path)) {
		std::string fileName = p.path().string();
		std::cout << fileName << " " << std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count() << std::endl;
	}
	std::cout << "Folder 2:\n";
	for (auto& p : std::filesystem::directory_iterator(path2)) {
		std::string fileName = p.path().string();
		std::cout << fileName << " " << std::chrono::duration_cast<std::chrono::milliseconds>(p.last_write_time().time_since_epoch()).count() << std::endl;
	}
    return a.exec();
}

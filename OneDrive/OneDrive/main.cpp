#include "Window.h"
#include "TCP_Client.h"
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QString>
#include "Folder.h"
#include <thread>
#include "Metadata.h"
void initialisations() {
	std::string path = "./StoredFiles";
	if (!std::filesystem::exists(path))
		std::filesystem::create_directory(path);
	path = "./StoredServerFiles";
	if (!std::filesystem::exists(path))
		std::filesystem::create_directory(path);
	if (!std::filesystem::exists("./Usernames.json"))
	{
		std::ofstream file("Usernames.json");
		std::string temp = R"({"user": [] })";
		file << temp;
	}
}
int main(int argc, char* argv[])
{
	initialisations();
	QApplication a(argc, argv);
	Window w;
	Client client;
	w.show();
	return a.exec();
}

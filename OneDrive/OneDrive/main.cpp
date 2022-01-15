#include "Window.h"
#include "TCP_Client.h"
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QString>
#include "Folder.h"
#include <thread>
#include "Metadata.h"
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Window w;
	Client client;
	w.show();
	return a.exec();
}

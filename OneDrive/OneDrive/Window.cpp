#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include "Folder.h"

int Window::confirmationDownload()
{
	QMessageBox::StandardButton resBtn =
		QMessageBox::question(this, "Download", tr("Do you want to download a folder?\n"),
			QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
			QMessageBox::Yes);
	if (resBtn == QMessageBox::Yes)
	{
		return 1;
	}
	else
		if (resBtn == QMessageBox::No)
		{
			return 0;
		}
		else
		{
			return -1;
		}
}

std::string Window::selectFolder()
{
	QString folder;
	folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	std::string path = folder.toStdString();
	return path;
}
void Window::Server(std::string command) {
	Client client;
	std::string temp = getUser();
	char* username = &temp[0];
	std::string path = "./StoredServerFiles";
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	path += "/";
	path += username;
	client.connectServer();
	client.sendUser(client.getSock(), username);
	client.sendUser(client.getSock(), &command[0]);
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
		if (fileName == "/eof" || fileType == "eof")
			ok = 1;
		if (ok != 1)
		{
			strcpy(fileType, client.recvFileName(client.getSock()));
			std::cout << fileName << std::endl << fileType << std::endl;
			if (strstr(fileType, "folder"))
			{
				std::filesystem::create_directory(fileName);
			}
			else
			{
				long size = client.recvFileSize(client.getSock());
				if (size > 0)
				{

					if (client.writeToFile(client.getSock(), fileName, size) == 0)
						ok = 1;

				}
				else if (size == 0)
				{
					std::ofstream file;
					file.open(fileName);
				}
				else if (size == -1)
					ok = 1;
			}
		}
	} while (ok != 1);
	int flush = 0;
	char fileRequested[FILENAME_MAX];
	do
	{
		flush = recv(client.getSock(), fileRequested, FILENAME_MAX, 0);
		std::cout << flush << std::endl;
	} while (flush > 0);
	closesocket(client.getSock());
	WSACleanup();
}
void Window::LoginToAccount()
{
	userName = ui.Username->text().toStdString();
	if (userName != "")
	{
		std::string path;
		std::unordered_set<std::string> verifySet = databaseUsername.getUniqueUserNames();
		if (verifySet.find(userName) != verifySet.end())
		{
			std::string pathGlobal = "./StoredServerFiles/";
			switch (confirmationDownload())		 //function return 1 for true, 0 for false and -1 for cancel
			{
			case 1:
				pathGlobal += userName;
				std::filesystem::remove_all(pathGlobal);
				std::filesystem::create_directory(pathGlobal);
				Server("download");
				break;
			case 0:
				pathGlobal += userName;
				std::filesystem::remove_all(pathGlobal);
				std::filesystem::create_directory(pathGlobal);
				Server("delete");
				path = selectFolder();
				copyDirectoryContents(path, pathGlobal);
				pathGlobal = "./StoredFiles/";
				pathGlobal += userName;
				std::filesystem::remove_all(pathGlobal);
				std::filesystem::create_directory(pathGlobal);
				copyDirectoryContents(path, pathGlobal);
				/*Server("upload");*/
				break;

			default:
				return;
				break;
			}

			Account* back = new Account(userName);
			back->setFixedSize(1200, 700);
			back->show();
			this->hide();

		}
		else
		{
			ui.userSucces->setVisible(false);
			ui.userTakenIcon->setVisible(true);
			ui.UserTaken->setText("Please enter a valid name!");
		}

	}
	else
	{
		ui.userSucces->setVisible(false);
		ui.userTakenIcon->setVisible(true);
		ui.UserTaken->setText("Please enter a valid name!");
	}

}




void Window::Signup()
{
	userName = ui.Username->text().toStdString();
	if (userName != "")
	{

		std::unordered_set<std::string> updateSet = databaseUsername.getUniqueUserNames();
		if (updateSet.find(userName) == updateSet.end())
		{
			ui.userTakenIcon->clear();
			if (ui.UserTaken->isVisible() == false)
				ui.UserTaken->setVisible(true);
			std::string message = "You have succesfully registered, " + userName + " !";
			ui.UserTaken->setText(QString::fromStdString(message));
			ui.userSucces->setVisible(true);
			databaseUsername.addNewUser(userName);
			databaseUsername.writeUsersToFile();
			updateSet.insert(userName);
			databaseUsername.setUniqueUsernames(updateSet);
		}
		else
		{
			ui.userSucces->setVisible(false);
			ui.userTakenIcon->setVisible(true);
			ui.UserTaken->setVisible(true);
			ui.UserTaken->setText("The user name is already taken!");
		}


	}
	else
	{
		ui.userSucces->setVisible(false);
		ui.userTakenIcon->setVisible(true);
		ui.UserTaken->setText("Please enter a valid name!");
	}

}



void Window::initializeJson()
{
	databaseUsername.setPath("./Usernames.json");
	databaseUsername.setFileName("Usernames.json");
	databaseUsername.inputJson("Usernames.json");
	std::unordered_set<std::string> aux;
	std::vector<nlohmann::json> temp;
	for (int i = 0; i < databaseUsername.getBody()["user"].size(); i++) {
		temp.push_back(databaseUsername.getBody()["user"][i]);
		std::string toString = databaseUsername.getBody()["user"][i]["username"];
		aux.insert(toString);
	}
	databaseUsername.setUserNames(temp);
	databaseUsername.setUniqueUsernames(aux);


}


Window::Window(QWidget* parent)
{
	initializeJson();
	ui.setupUi(this);
	ui.userTakenIcon->setVisible(false);
	ui.UserTaken->setVisible(false);
	ui.userSucces->setVisible(false);
	QObject::connect(ui.Username, SIGNAL(returnPressed()), SLOT(LoginToAccount()));





}


#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include "Folder.h"
#include "Metadata.h"
int Window::confirmationDownload()
{
	QMessageBox::StandardButton resBtn =QMessageBox::question(this, "Download", tr("Do you want to download a folder?\n"),
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

bool Window::checkUsername(std::string user)
{
	const std::regex txt_regex("[a-z]+[a-z0-9]*");
	if (std::regex_match(user, txt_regex))
		return true;
	return false;

}

std::string Window::selectFolder()
{
	std::string path = "";
	QString folder;
	folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly);
	if (!folder.isNull())
	{
		 path = folder.toStdString();
	}
	return path;
}
bool Window::downloadServer(Client client) {

	while (true)
	{
		char fileName[FILENAME_MAX];
		char fileType[FILENAME_MAX];
		strcpy(fileName, client.recvFileName(client.getSock()));
		strcpy(fileType, client.recvFileName(client.getSock()));
		if (strstr(fileName, "NULL") || strstr(fileType, "NULL"))
			return 0;
		if (strstr(fileName, "/eof") || strstr(fileType, "/eof"))
			return 1;
		if (strstr(fileType, "folder"))
		{
			std::filesystem::create_directory(fileName);
		}
		else if (strstr(fileType, "file"))
		{
			long size = client.recvFileSize(client.getSock());
			if (size > 0)
			{

				if (client.writeToFile(client.getSock(), fileName, size) == 0)
					return 0;

			}
			else if (size == 0)
			{
				std::ofstream file;
				file.open(fileName);
			}
			else if (size == -1)
				return 0;
		}
	}
}
bool Window::uploadServer(Client client, std::string path) {
	if (std::filesystem::exists(path))
	{
		for (const auto& it : std::filesystem::recursive_directory_iterator(path))
		{
			if (it.is_directory())
			{
				char fileName[FILENAME_MAX];
				strcpy(fileName, it.path().string().c_str());
				if (client.sendFileName(client.getSock(), fileName) == 0)
					return 0;
				char fileType[FILENAME_MAX];
				strcpy(fileType, "folder");
				if (client.sendFileName(client.getSock(), fileType) == 0)
					return 0;
			}
			else
			{
				char fileName[FILENAME_MAX];
				strcpy(fileName, it.path().string().c_str());
				if (client.sendFileName(client.getSock(), fileName) == 0)
					return 0;
				char fileType[FILENAME_MAX];
				strcpy(fileType, "file");
				if (client.sendFileName(client.getSock(), fileType) == 0)
					return 0;
				if (client.sendFile(client.getSock(), it.path().string()) == 0)
					return 0;
			}

		}
		char fileName[FILENAME_MAX];
		strcpy(fileName, "/eof");
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;
		return 1;
	}
	else
	{
		std::filesystem::create_directory(path);
		return 1;
	}
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
	int cases;
	if (command == "delete")
		cases = 0;
	else if (command == "download")
		cases = 1;
	else if (command == "upload")
		cases = 2;
	switch (cases)
	{
	case 0:
		//deleteServer();
		break;
	case 1:
		downloadServer(client);
		break;
	case 2:
		uploadServer(client, path);
		break;
	}
	closesocket(client.getSock());
	WSACleanup();
}
void Window::LoginToAccount()
{
	userName = ui.Username->text().toStdString();
	std::transform(userName.begin(), userName.end(), userName.begin(), ::tolower);
	if (userName != "")
	{
		std::string path;
		std::unordered_set<std::string> verifySet = databaseUsername.getUniqueUserNames();
		Metadata metadata;
		if (verifySet.find(userName) != verifySet.end())
		{
			std::string pathGlobal = "./StoredServerFiles/";
			std::string pathLocal = "./StoredFiles/";
			switch (confirmationDownload())		 //function return 1 for true, 0 for false and -1 for cancel
			{
			case 0:
				pathGlobal += userName;
				std::filesystem::remove_all(pathGlobal);
				std::filesystem::create_directory(pathGlobal);
				path = selectFolder();
				if (path == "")
					return;
				copyDirectoryContents(path, pathGlobal);
				metadata.folderMetadata(pathGlobal);
				metadata.outputJson(pathGlobal + "/metadata.json");
				pathGlobal = "./StoredFiles/";
				pathGlobal += userName;
				std::filesystem::remove_all(pathGlobal);
				std::filesystem::create_directory(pathGlobal);
				copyDirectoryContents(path, pathGlobal);
				metadata.outputJson(pathGlobal + "/metadata.json");
				Server("upload");
				break;
			case 1:
				pathGlobal += userName;
				std::filesystem::remove_all(pathGlobal);
				std::filesystem::create_directory(pathGlobal);
				Server("download");
				pathLocal += userName;
				std::filesystem::remove_all(pathLocal);
				std::filesystem::create_directory(pathLocal);
				copyDirectoryContents(pathGlobal, pathLocal);
				//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				metadata.folderMetadata(pathLocal);
				metadata.outputJson(pathLocal + "/metadata.json");
				metadata.outputJson(pathGlobal + "/metadata.json");
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
	std::transform(userName.begin(), userName.end(), userName.begin(), ::tolower);
	if (!checkUsername(userName))
	{
		ui.userSucces->setVisible(false);
		ui.userTakenIcon->setVisible(true);
		ui.UserTaken->setText("Please enter a valid name!");
		return;
	}
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


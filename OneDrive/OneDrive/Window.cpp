#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>
#include <fstream>
#include <iostream>
#include <QFileDialog>


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

void Window::LoginToAccount()
{
	userName = ui.Username->text().toStdString();
	if (userName != "")
	{
		std::string path;
		std::unordered_set<std::string> verifySet = databaseUsername.getUniqueUserNames();
		if (verifySet.find(userName) != verifySet.end())
		{
			switch (confirmationDownload())		 //function return 1 for true, 0 for false and -1 for cancel
			{
			case 0:
				break;
			case 1:

				path = selectFolder();
				std::cout << path;
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


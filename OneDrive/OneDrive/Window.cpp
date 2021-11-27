#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>
#include <fstream>
#include <iostream>



void Window::LoginToAccount()
{
	userName = ui.Username->toPlainText().toStdString();
	if (userName != "")
	{

		std::unordered_set<std::string> verifySet = databaseUsername.getUniqueUserNames();
		if (verifySet.find(userName) != verifySet.end())
		{

			Account* back = new Account(userName);
			back->setFixedSize(700, 700);
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
	userName = ui.Username->toPlainText().toStdString();
	if (userName!= "")
	{
		
		std::unordered_set<std::string> updateSet= databaseUsername.getUniqueUserNames();
		if (updateSet.find(userName) == updateSet.end())
		{
			ui.userTakenIcon->clear();
			if(ui.UserTaken->isVisible()==false)
				ui.UserTaken->setVisible(true);
			std::string message = "You have succesfully registered, " + userName+" !";
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
	
	
	



}


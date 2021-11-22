#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>
#include "WindowAccount.h"
#include <fstream>
#include <iostream>


void Window::on_Button_clicked() // on click i take the username and store it
{
	if (!editUsername->text().isEmpty())
	{
		std::string username = editUsername->text().toStdString();

		label->setText("Name saved");
		WindowAccount* back = new WindowAccount(this);
		back->setFixedSize(700, 700);
		back->show();
		this->close();

	}
	else
		label->setText("Introduceti scris");

}

void Window::Signup()
{

	if (!editUsername->text().isEmpty())
	{
		std::string username = editUsername->text().toStdString();
		std::unordered_set<std::string> updateSet= databaseUsername.getUniqueUserNames();
		if (updateSet.find(username) == updateSet.end())
		{
			label->clear();
			databaseUsername.addNewUser(username);
			databaseUsername.writeUsersToFile();
			updateSet.insert(username);
			databaseUsername.setUniqueUsernames(updateSet);
		}
		else
		{
			label->setText("This user name already exists!");
		}


	}
	else
	{
		std::cout << "e empty" << std::endl;
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
	QWidget* ui_area = new QWidget;
	setCentralWidget(ui_area);
	QPushButton* login, * signUp;
	QHBoxLayout* mainLayout, * buttonLayout;
	QVBoxLayout* secondLayout;


	initializeJson();

	
	labelUsername = new QLabel("Username");
	signUp = new QPushButton("SignUp");
	login = new QPushButton("Login");
	editUsername = new QLineEdit();
	label = new QLabel();
	QLabel* welcome = new QLabel("Welcome to OneDrive!");

	editUsername->setMaximumSize(300, 20);
	setWindowTitle("Login");
	welcome->setStyleSheet("font-size:30px;font-weight:bold;");
	labelUsername->setAlignment(Qt::AlignBottom);
	label->setStyleSheet("color:red;font-size:20px;font: \"Times New Roman\", Times;");
	login->setFixedSize(100, 20);
	login->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");
	signUp->setFixedSize(100, 20);
	signUp->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");

	connect(login, SIGNAL(clicked()), SLOT(on_Button_clicked()));
	connect(signUp, SIGNAL(clicked()), SLOT(Signup()));
	

	mainLayout = new QHBoxLayout(this);
	secondLayout = new QVBoxLayout(this);
	buttonLayout = new QHBoxLayout(this);

	secondLayout->addWidget(welcome);
	secondLayout->addWidget(labelUsername);
	secondLayout->addWidget(editUsername);


	buttonLayout->addWidget(login);
	buttonLayout->addWidget(signUp);
	secondLayout->addLayout(buttonLayout);
	secondLayout->addWidget(label);
	mainLayout->addLayout(secondLayout, Qt::AlignCenter);



	ui_area->setLayout(mainLayout);



}


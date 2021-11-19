#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>
#include "WindowAccount.h"

JSON databaseUsername("./Usernames.json", "Usernames.json");

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

		databaseUsername.writeUsersWithoutOverwriting(username);
		
	}
	else
		label->setText("Introduceti scris");

}


Window::Window(QWidget* parent)
{
	QWidget* ui_area = new QWidget;
	setCentralWidget(ui_area);
	QPushButton* login, * signUp;
	
	QHBoxLayout* mainLayout, * buttonLayout;
	QVBoxLayout* secondLayout;

	setWindowTitle("Window test");

	labelUsername = new QLabel("Username");
	labelUsername->setAlignment(Qt::AlignBottom);
	signUp = new QPushButton("SignUp");
	login = new QPushButton("Login");
	editUsername = new QLineEdit();
	label = new QLabel();
	QLabel* welcome = new QLabel("Welcome to OneDrive!");
	welcome->setStyleSheet("font-size:30px;font-weight:bold;");

	setWindowTitle("Account Window");

	login->setFixedSize(100, 20);
	login->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");
	connect(login, SIGNAL(clicked()), SLOT(on_Button_clicked()));
	signUp->setFixedSize(100, 20);
	signUp->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");
	connect(signUp, SIGNAL(clicked()), SLOT(Signup()));
	editUsername->setMaximumSize(300, 20);
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


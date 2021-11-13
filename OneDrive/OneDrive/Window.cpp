#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>

void Window::on_Button_clicked() // on click i take the username and store it
{
    if (!editUsername->text().isEmpty())
    {
        std::string username = editUsername->text().toStdString();

        label->setText("Name saved");
    }
    else
        label->setText("Introduceti scris");

}


Window::Window(QWidget* parent)
{
    QWidget* ui_area = new QWidget;
    setCentralWidget(ui_area);
    QPushButton* login, * signUp;

    QHBoxLayout* mainLayout;
    QVBoxLayout* secondLayout;

    setWindowTitle("Window test");

    labelUsername = new QLabel("Username");
    labelUsername->setAlignment(Qt::AlignBottom);
    signUp = new QPushButton("SignUp");
    login = new QPushButton("Login");
    editUsername = new QLineEdit();
    label = new QLabel();

    login->setFixedSize(100, 20);
    login->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");
    connect(login, SIGNAL(clicked()), SLOT(on_Button_clicked()));
    signUp->setFixedSize(100, 20);
    signUp->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");
 
    editUsername->setMaximumSize(300, 20);
    mainLayout = new QHBoxLayout(this);
    secondLayout = new QVBoxLayout(this);


    secondLayout->addWidget(labelUsername);
    secondLayout->addWidget(editUsername);

    secondLayout->addWidget(login);
    secondLayout->addWidget(signUp);
    secondLayout->addWidget(label);
    mainLayout->addLayout(secondLayout, Qt::AlignCenter);



    ui_area->setLayout(mainLayout);



}


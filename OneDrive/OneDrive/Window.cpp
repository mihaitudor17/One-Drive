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
    QPushButton* button;
    
    QHBoxLayout* mainLayout;
    QVBoxLayout* secondLayout;
   
    setWindowTitle("Window test");
    
    labelUsername = new QLabel("Username");
    button = new QPushButton("Login");
    editUsername = new QLineEdit();
    label = new QLabel();

    button->setFixedSize(100, 20);
    button->setStyleSheet("border-radius:10px;background-color:grey;border:3 px solid black;");
    connect(button, SIGNAL(clicked()), SLOT(on_Button_clicked()));
   
    mainLayout = new QHBoxLayout(this); 
    secondLayout = new QVBoxLayout(this);


    secondLayout->addWidget(labelUsername);
    secondLayout->addWidget(editUsername, Qt::AlignCenter);
    
    secondLayout->addWidget(button, Qt::AlignCenter);
    secondLayout->addWidget(label, Qt::AlignCenter);
    mainLayout->addLayout(secondLayout, Qt::AlignCenter);
   
    
  
    ui_area->setLayout(mainLayout);
    
   

}


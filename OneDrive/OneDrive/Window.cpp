#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>





Window::Window(QWidget* parent)
{
    QWidget* ui_area = new QWidget;
    setCentralWidget(ui_area);
    QPushButton* button;
    
    QHBoxLayout* mainLayout;
   
    setWindowTitle("Window test");
    
    labelUsername = new QLabel("Username");
    button = new QPushButton("Login");
    editUsername = new QLineEdit();
    label = new QLabel();

   
    mainLayout = new QHBoxLayout(this); 
   
    mainLayout->addWidget(labelUsername);
    mainLayout->addWidget(editUsername, Qt::AlignCenter);
    
    mainLayout->addWidget(button, Qt::AlignCenter);
    mainLayout->addWidget(label, Qt::AlignCenter);
   
    
  
    ui_area->setLayout(mainLayout);
    
   

}



#include "WindowAccount.h"
#include "Window.h"
#include <QMessageBox>
#include <QLineEdit>

WindowAccount::WindowAccount(QWidget* parent)
{
    QWidget* ui_area = new QWidget;
    setCentralWidget(ui_area);
    QVBoxLayout* mainLayout;


    QLabel* welcome = new QLabel("Welcome to OneDrive!");
    welcome->setStyleSheet("font-size:30px;font-weight:bold;");
    welcome->setAlignment(Qt::AlignHCenter);
    setWindowTitle("Account Window");



    mainLayout = new QVBoxLayout(this);





    mainLayout->addWidget(welcome, Qt::AlignCenter);



    ui_area->setLayout(mainLayout);



}
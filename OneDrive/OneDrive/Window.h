#pragma once
#include "JSON.h"
#include <QtWidgets/QMainWindow>
#include "ui_OneDrive.h"
#include <QVBoxLayout>
#include <QLabel>
#include <unordered_set>
#include "Account.h"
#include "TCP_Client.h"
class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = 0);
    private slots:
    void LoginToAccount();
    int confirmationDownload();
    std::string selectFolder();
    void Signup();
    void Server(std::string);
    std::string getUser()
    {
        return userName;
    }
   

private:
    Ui::OneDriveClass ui;
    std::string userName;
    JSON databaseUsername;
    void initializeJson();
};

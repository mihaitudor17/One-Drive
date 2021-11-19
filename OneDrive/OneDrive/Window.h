#pragma once
#include "JSON.h"
#include <QtWidgets/QMainWindow>
#include "ui_OneDrive.h"
#include <QVBoxLayout>
#include <QLabel>
#include <unordered_set>
class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = 0);

    private slots:
    void on_Button_clicked();
    void Signup();
    void Window::readJsonInSet(std::unordered_set<std::string>& uniqueUsernames);

private:
    
    std::unordered_set<std::string> uniqueUsernames;
    JSON databaseUsername;
    QLabel* labelUsername;
    QLineEdit* editUsername;
    QLabel* label;
};

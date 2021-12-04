#pragma once
#include <QWidget>
#include "ui_Account.h"
#include <filesystem>
#include <QLabel>
#include <QPushButton>

class Account : public QWidget
{
    Q_OBJECT

public:
    Account(const std::string& userName, QWidget* parent = 0);


private:
    Ui::Account ui;
    std::string userName;
    void showContentsOfFile();
private slots:
    void openFile();
};
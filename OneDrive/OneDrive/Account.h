#pragma once
#include <QWidget>
#include "ui_Account.h"
#include <filesystem>
#include <QLabel>

class Account : public QWidget
{
	Q_OBJECT

public:
	Account(const std::string& userName, QWidget* parent = 0);

private slots:
	void back_folder_local();

private:
	std::filesystem::path pathLocal;
	std::filesystem::path pathGlobal;
	std::string fileNameGlobal;
	Ui::Account ui;
	std::string userName;
	void showContentLocal();
	void showContentServer();
};

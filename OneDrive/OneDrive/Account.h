#pragma once
#include <QWidget>
#include "ui_Account.h"
#include <filesystem>
#include <QLabel>
class Account : public QWidget
{
	Q_OBJECT


 signals:
	void renameFileSignal(std::string selected);
	void deleteFileSignal(std::string selected);
private slots:
	void back_folder_local();
	void back_folder_server();
	void renameFileSlot(std::string selected);
	void renameLocalSendSignal();
	void deleteLocalSendSignal();
	void deleteFileSlot(std::string selected);

private:
	QPushButton* labelToBeDeselected;
	std::string selected;
	std::filesystem::path pathLocal;
	std::filesystem::path pathGlobal;
	std::string fileNameGlobal;
	Ui::Account ui;
	std::string userName;
	void checkLayout(QWidget* currentWidget);
	void showContentLocal();
	void showContentServer();

public:
	Account(const std::string& userName, QWidget* parent = 0);
	std::string getUser()
	{
		return userName;
	}
	void downloadServer();
};

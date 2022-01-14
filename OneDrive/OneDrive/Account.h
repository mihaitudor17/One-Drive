#pragma once
#include <QWidget>
#include "ui_Account.h"
#include <filesystem>
#include <QGridLayout>
#include <QLabel>
#include "Window.h"
#include "Polling.h"
class Account : public QWidget
{
	Q_OBJECT


signals:
	void renameFileSignal(std::string selected);
	void deleteFileSignal(std::string selected);
private slots:
	void addFolder();
	void download();
	void backFolderLocal();
	void backFolderServer();
	void renameFileSlot(std::string selected);
	void renameLocalSendSignal();
	void deleteLocalSendSignal();
	void refreshLocal();
	void refreshServer();
	void deleteFileSlot(std::string selected);
	void refresh();
	void polling();

private:
	QPushButton* labelToBeDeselected;
	std::string selected;
	std::filesystem::path pathLocal;
	std::filesystem::path pathGlobal;
	std::string fileNameGlobal;
	Ui::Account ui;
	std::string userName;

	Polling pollingVariable;

	void checkLayout(QWidget* currentWidget);
	void showContentLocal();
	void showContentServer();
	void Server(std::string);

	QPixmap gridLine(QPixmap pixmap, QLabel* image, QGridLayout* gridLocal, QPushButton* label, int contorServerGrid, std::filesystem::directory_entry file);

	QPixmap gridLayout(QPixmap pixmap, QLabel* image, QGridLayout* gridLocal, QPushButton* label, int contorServerGrid, std::filesystem::directory_entry file);

public:
	Account(const std::string& userName, QWidget* parent = 0);
	std::string getUser();


};

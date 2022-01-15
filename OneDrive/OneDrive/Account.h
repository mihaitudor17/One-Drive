#pragma once
#include <QWidget>
#include "ui_Account.h"
#include <filesystem>
#include <QGridLayout>
#include <QLabel>
#include "Window.h"
#include "Polling.h"
#include "Folder.h"
#include <chrono>
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
	void refreshTrash();
	void deleteFileSlot(std::string selected);
	void refresh();
	void polling();
	void showTrash();
	void restore();
	void changeTrashLogo();
	void addFile();
	void deleteLocal();
private:
	QPushButton* labelToBeDeselected;
	std::string selected;
	QPushButton* labelToBeDeselectedTrash;
	std::string selectedTrash;
	std::filesystem::path pathLocal;
	std::filesystem::path pathGlobal;
	std::filesystem::path trashPath;
	std::string fileNameGlobal;
	Ui::Account ui;
	std::string userName;
	Polling pollingVariable;
	bool serverOrTrash;

	const int iconsOnPage = 9;

	void checkLayout(QWidget* currentWidget);
	void showContentLocal();
	void showContentServer();
	void showContentTrash();
	void Server(std::string);

	void checkTrash();

	QPixmap gridLine(QPixmap pixmap, QLabel* image, QGridLayout* gridLocal, QPushButton* label, int contorServerGrid, std::filesystem::directory_entry file);

	QPixmap gridLayout(QPixmap pixmap, QLabel* image, QGridLayout* gridLocal, QPushButton* label, int contorServerGrid, std::filesystem::directory_entry file);

	bool checkRezervedFiles(std::filesystem::directory_entry file);

public:
	void startup();
	Account(const std::string& userName, QWidget* parent = 0);
	std::string getUser();


};
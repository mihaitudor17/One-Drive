#pragma once
#include <QWidget>
#include "ui_Account.h"
#include <filesystem>
#include <QGridLayout>
#include <QLabel>
#include "Window.h"
#include "Polling.h"
#include "Folder.h"
#include "Metadata.h"
#include <chrono>
#include <cstdint>

enum class ServerCommand : uint8_t {
	DELETE_FILE,
	DOWNLOAD_FILE,
	UPDATE_FILE,
	RENAME_FILE
};

class Account : public QWidget
{
	Q_OBJECT

signals:
	void renameFileSignal( std::string& selected);
	void deleteFileSignal( std::string& selected);

private slots:
	void addFolder();
	void backFolderLocal();
	void backFolderServer();
	void renameFileSlot( std::string& selected);
	void renameLocalSendSignal();
	void deleteLocalSendSignal();
	void refreshLocal();
	void refreshServer();
	
	void refreshTrash();
	void deleteFileSlot( std::string& selected);
	void refresh();
	void polling();
	void showTrash();
	void restore();
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
	QMutex mutex;
	const uint8_t iconsOnPage = 9;

	void checkLayout(QWidget* currentWidget);
	void showContentLocal();
	void showContentServer();
	void showContentTrash();
	void splitFileName(const std::string& fileName, std::string& extension, std::string& name);
	void checkTrash();
	void startup();
	QPixmap gridLine(QPixmap pixmap, QLabel* image, QGridLayout* grid, QPushButton* label, uint16_t contorServerGrid, std::filesystem::directory_entry file);
	QPixmap gridLayout(QPixmap pixmap, QLabel* image, QGridLayout* grid, QPushButton* label, uint16_t contorServerGrid, std::filesystem::directory_entry file);
	void syncFolderWithMetadata(const std::filesystem::path& path, const Metadata& metadata);
	bool checkRezervedFiles(std::filesystem::directory_entry file);

public:
	
	void Server(ServerCommand, std::string, std::string);
	Account(const std::string& userName, QWidget* parent = 0);
	std::string getUser();
	
};
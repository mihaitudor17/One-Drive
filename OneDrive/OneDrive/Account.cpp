#include "Account.h"
#include<QPushButton>
#include <iostream>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include "TCP_Client.h"
#include "FowlerNollVo.h"
#include "Metadata.h"
#include <QMutex>
#include "TCP_Client.h"
#include <cstdint>

std::string ServerCommandTypeToString(ServerCommand command)
{
	switch (command)
	{
	case ServerCommand::DELETE_FILE:
		return "deleteFile";
		break;
	case ServerCommand::DOWNLOAD_FILE:
		return "downloadFile";
		break;
	case ServerCommand::UPDATE_FILE:
		return "updateFile";
		break;
	case ServerCommand::RENAME_FILE:
		return "renameFile";
		break;
	default:
		break;
	}
}

void Account::backFolderLocal()
{
	std::string path_aux = pathLocal.string();
	if (path_aux == ("./StoredFiles/" + userName))
	{
		//throw "Back out of the restricted area";
		return;
	}
	while (path_aux[path_aux.length() - 1] != '/')
	{
		path_aux.erase(path_aux.begin() + path_aux.length() - 1);
	}
	path_aux.erase(path_aux.begin() + path_aux.length() - 1);

	pathLocal = path_aux;
	refreshLocal();
}

void Account::backFolderServer()
{
	if (!serverOrTrash)
	{
		std::string pathAux = pathGlobal.string();
		if (pathAux == ("./StoredServerFiles/" + userName))
		{
			return;
		}
		while (pathAux[pathAux.length() - 1] != '/')
		{
			pathAux.erase(pathAux.begin() + pathAux.length() - 1);
		}
		pathAux.erase(pathAux.begin() + pathAux.length() - 1);
		pathGlobal = pathAux;
		refreshServer();
	}

	else
	{
		std::string pathAuxTrash = trashPath.string();
		if (pathAuxTrash == ("./StoredFiles/" + userName + "/Recycle Bin"))
		{
			return;
		}
		while (pathAuxTrash[pathAuxTrash.length() - 1] != '/')
		{
			pathAuxTrash.erase(pathAuxTrash.begin() + pathAuxTrash.length() - 1);
		}
		pathAuxTrash.erase(pathAuxTrash.begin() + pathAuxTrash.length() - 1);
		trashPath = pathAuxTrash;
		refreshTrash();
	}
}

void Account::renameFileSlot(std::string& selected)
{
	if (selected != "")
	{
		bool ok;
		QString text = QInputDialog::getText(this, tr("New name"),
			tr("Please insert a new name:"), QLineEdit::Normal,
			QDir::home().dirName(), &ok, Qt::MSWindowsFixedSizeDialogHint);

		if (ok && !text.isEmpty())
		{
			bool found = false;
			for (const auto& file : std::filesystem::directory_iterator(pathLocal))
			{
				if (file.path().filename() == text.toStdString())
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				if (std::filesystem::is_directory(pathLocal / selected))
				{
					std::filesystem::rename(pathLocal / selected, pathLocal / text.toStdString());
					refreshLocal();
				}
				else
				{
					std::string extension;
					for (uint16_t position = selected.find_last_of('.'); position < selected.length(); position++)
					{
						extension.push_back(selected[position]);
					}
					std::filesystem::rename(pathLocal / selected, pathLocal / (text.toStdString() + extension));
					refreshLocal();
				}
			}
			else
			{
				QMessageBox::warning(this, "Alert!", "This file already exists!\nPlease try another name.");
			}
		}
	}
	else
	{
		QMessageBox::warning(this, "Alert!", "Please select a file.");
	}

	selected = "";
}

void Account::addFolder()
{
	int folderNumber = 1;
	QString folder;
	folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly);
	if (!folder.isNull())
	{
		std::filesystem::path filePath = folder.toStdString();
		if (!std::filesystem::exists("./StoredFiles/" + userName + "/" + filePath.filename().string()))
		{
			std::filesystem::create_directory("./StoredFiles/" + userName + "/" + filePath.filename().string());
			copyDirectoryContents(filePath, "./StoredFiles/" + userName + "/" + filePath.filename().string());
		}
		else
		{
			while (std::filesystem::exists("./StoredFiles/" + userName + "/" + filePath.filename().string() + "(" + std::to_string(folderNumber) + ")"))
			{
				folderNumber++;
			}
			std::filesystem::create_directory("./StoredFiles/" + userName + "/" + filePath.filename().string() + "(" + std::to_string(folderNumber) + ")");
			copyDirectoryContents(filePath, "./StoredFiles/" + userName + "/" + filePath.filename().string() + "(" + std::to_string(folderNumber) + ")");


		}

	}
	refresh();

}

void Account::renameLocalSendSignal()
{
	emit renameFileSignal(selected);
}

void Account::deleteLocalSendSignal()
{
	emit deleteFileSlot(selected);
}

void  Account::refreshLocal()
{
	qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	showContentLocal();
}

void  Account::refreshServer()
{
	qDeleteAll(ui.serverWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	showContentServer();
}
void Account::splitFileName(const std::string &fileName, std::string& extension, std::string& name)
{
	bool extensionStart = false;
	for (int i = 0; i < fileName.length(); i++)
	{
		if (fileName[i] == '.')
			extensionStart = true;
		if (extensionStart)
			extension += fileName[i];
		else
			name += fileName[i];
	}
}
void Account::refreshTrash()
{
	qDeleteAll(ui.serverWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	showContentTrash();
}
void Account::deleteFileSlot(std::string& selected)
{
	if (selected != "")
	{
		if (std::filesystem::is_directory(pathLocal / selected))
		{
			if (!std::filesystem::exists(trashPath / selected))
			{
				std::filesystem::create_directory(trashPath / selected);      //if folder's name dosent exists
				copyDirectoryContents(pathLocal / selected, trashPath / selected);
				std::filesystem::remove_all((pathLocal / selected));
			}
			else
			{
				std::string newName = selected + "(1)";
				std::filesystem::create_directory(trashPath / newName);      //if folder's name dosent exists
				copyDirectoryContents(pathLocal / selected, trashPath / newName);
				std::filesystem::remove_all((pathLocal / selected));
			}
		}
		else
		{
			if (!std::filesystem::exists(trashPath / selected))
			{
				std::filesystem::copy(pathLocal / selected, trashPath);
				std::filesystem::remove_all((pathLocal / selected));
			}

			else
			{															//if 2 file with the sa me name exists
				std::string extension, fileName, newName;
				splitFileName(selected, extension, fileName);
				newName = fileName + "(1)" + extension;
				std::filesystem::rename(pathLocal / selected, pathLocal / newName);
				std::filesystem::copy(pathLocal / newName, trashPath);
				std::filesystem::remove_all((pathLocal / newName));
			}
		}

	}

	else
		QMessageBox::warning(this, "Alert!", "Please select a file to delete!");
	refreshLocal();
	selected = "";
}




void Account::refresh()
{
	refreshLocal();
	if (!serverOrTrash)
		refreshServer();
	else
		refreshTrash();
}

bool deleteFile(Client client, std::string path)
{
	if (client.sendFileName(client.getSock(), &path[0]) == 0)
		return 0;
	return 1;
}

bool downloadFile(Client client, std::string path)
{
	if (client.sendFileName(client.getSock(), &path[0]) == 0)
		return 0;

	while (true)
	{
		char fileName[FILENAME_MAX];
		strcpy(fileName, client.recvFileName(client.getSock()));

		if (strstr(fileName, "NULL"))
			return 0;

		if (fileName == "/eof")
			return 1;

		long size = client.recvFileSize(client.getSock());

		if (size > 0)
		{
			if (client.writeToFile(client.getSock(), fileName, size) == 0)
				return 0;
		}

		else if (size == 0)
		{
			std::ofstream file;
			file.open(fileName);
		}

		else if (size == -1)
			return 0;
	}
}

bool updateFile(Client client, std::filesystem::path it)
{
	if (std::filesystem::is_directory(it))
	{
		char fileName[FILENAME_MAX];
		strcpy(fileName, it.string().c_str());

		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;

		char fileType[FILENAME_MAX];
		strcpy(fileType, "folder");

		if (client.sendFileName(client.getSock(), fileType) == 0)
			return 0;

		for (const auto& iterator : std::filesystem::recursive_directory_iterator(it))
		{
			if (iterator.is_directory())
			{
				strcpy(fileName, iterator.path().string().c_str());
				if (client.sendFileName(client.getSock(), fileName) == 0)
					return 0;

				char fileType[FILENAME_MAX];
				strcpy(fileType, "folder");
				if (client.sendFileName(client.getSock(), fileType) == 0)
					return 0;
			}
			else
			{
				strcpy(fileName, iterator.path().string().c_str());
				if (client.sendFileName(client.getSock(), fileName) == 0)
					return 0;

				char fileType[FILENAME_MAX];
				strcpy(fileType, "file");
				if (client.sendFileName(client.getSock(), fileType) == 0)
					return 0;

				if (client.sendFile(client.getSock(), iterator.path().string()) == 0)
					return 0;
			}

		}
		strcpy(fileName, "/eof");
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;

		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;
	}
	else
	{
		char fileName[FILENAME_MAX];
		strcpy(fileName, it.string().c_str());
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;

		char fileType[FILENAME_MAX];
		strcpy(fileType, "file");
		if (client.sendFileName(client.getSock(), fileType) == 0)
			return 0;

		if (client.sendFile(client.getSock(), it.string()) == 0)
			return 0;

		strcpy(fileName, "/eof");
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;

		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;
	}
	return 1;

}

bool renameFile(Client client, std::string oldName, std::string newName)
{
	if (client.sendFileName(client.getSock(), &oldName[0]) == 0)
		return 0;

	if (client.sendFileName(client.getSock(), &newName[0]) == 0)
		return 0;

	return 1;
}

void Account::syncFolderWithMetadata(const std::filesystem::path& path, const Metadata& metadata) {
	FowlerNollVo hashFunction;
	std::unordered_set<long long> hashes;
	for (const auto& it : std::filesystem::directory_iterator(path))
	{
		if (it.path().filename().string() != "metadata.json" && it.path().filename().string() != "Recycle Bin")
		{

			//daca exista fisierul
			if (metadata.m_body.find(it.path().filename().string()) != metadata.m_body.end()) {
				long long lastWriteTime = std::chrono::duration_cast<std::chrono::milliseconds>(it.last_write_time().time_since_epoch()).count();
				if (lastWriteTime - 10 > metadata.m_body[it.path().filename().string()]["lastWriteTime"]) {
					if (std::filesystem::exists(pathGlobal / it.path().filename())) {
						if (std::filesystem::is_directory(it.path())) {
							std::filesystem::remove_all(pathGlobal / it.path().filename());
							std::filesystem::create_directory(pathGlobal / it.path().filename());
							copyDirectoryContents(it.path(), pathGlobal / it.path().filename());
						}
						else {
							std::filesystem::remove(pathGlobal / it.path().filename());
							std::filesystem::copy(it.path(), pathGlobal);
						}
					}
					Server(ServerCommand::UPDATE_FILE, it.path().string(), "");
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					Metadata metadata;
					std::string pathGlobal = "./StoredServerFiles/";
					pathGlobal += userName;
					metadata.folderMetadata(path.string());
					metadata.outputJson(pathGlobal + "/metadata.json");
				}
			}
			else {//cazul de redenumire
				int rename = 0;
				std::string renamed;
				std::string renamedPath;
				for (const auto& item : metadata.m_body.items()) {
					if (std::filesystem::is_directory(it.path())) {
						size_t hash = hashFunction.getHashOfFolder(it.path().string());
						if (item.value()["hash"] == hash) {
							rename = 1;
							hashes.insert(hash);
							renamed = item.key();
							renamedPath = it.path().string();
							renamedPath = renamedPath.substr(0, renamedPath.find_last_of('\\'));
							renamedPath += "/";
							renamedPath += renamed;
							break;
						}
					}
					else if (it.path().filename().string().find(".txt") != std::string::npos)
					{
						size_t hash = hashFunction.hashingTextFile(it.path().string());
						if (item.value()["hash"] == hash) {
							rename = 1;
							hashes.insert(hash);
							renamed = item.key();
							renamedPath = it.path().string();
							renamedPath = renamedPath.substr(0, renamedPath.find_last_of('\\'));
							renamedPath += "/";
							renamedPath += renamed;
							break;
						}

					}
					else
						if (it.path().filename().string().find(".mp4") != std::string::npos ||
							it.path().filename().string().find(".jpg") != std::string::npos ||
							it.path().filename().string().find(".png") != std::string::npos
							) {
							size_t hash = hashFunction.hashingImageFileAndVideoFile(it.path().string());
							if (item.value()["hash"] == hash) {
								rename = 1;
								hashes.insert(hash);
								renamed = item.key();
								renamedPath = it.path().string();
								renamedPath = renamedPath.substr(0, renamedPath.find_last_of('\\'));
								renamedPath += "/";
								renamedPath += renamed;
								break;
							}
						}

				}
				if (rename) {
					Server(ServerCommand::RENAME_FILE, renamed, it.path().filename().string());
					if (std::filesystem::exists(pathGlobal / it.path().filename().string())) {
						std::filesystem::rename(pathGlobal / renamed, pathGlobal / it.path().filename().string());
					}
					Metadata metadata;
					std::string pathGlobal = "./StoredServerFiles/";
					pathGlobal += userName;
					metadata.folderMetadata(path.string());
					metadata.outputJson(pathGlobal + "/metadata.json");
				}
				else {//nu exista pana acum
					if (std::filesystem::is_directory(it.path())) {
						std::filesystem::create_directory(pathGlobal / it.path().filename().string());
						copyDirectoryContents(it.path(), pathGlobal / it.path().filename());
					}
					else {
						if (!std::filesystem::exists(pathGlobal / it.path().filename())) {
							std::filesystem::copy(it.path(), pathGlobal);
						}
					}
					if (std::filesystem::exists(pathGlobal / it.path().filename())) {
						Server(ServerCommand::UPDATE_FILE, it.path().string(), "");
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					Metadata metadata;
					std::string pathGlobal = "./StoredServerFiles/";
					pathGlobal += userName;
					metadata.folderMetadata(path.string());
					metadata.outputJson(pathGlobal + "/metadata.json");
				}
			}
		}
	}
	//cazul de stergere
	for (const auto& item : metadata.m_body.items()) {
		int name = 1, hash = 1;
		if (!std::filesystem::exists(path / item.key())) {
			name = 0;
		}
		if (hashes.find(item.value()["hash"]) == hashes.end()) {
			hash = 0;
		}
		if (name == 0 && hash == 0) {
			if (std::filesystem::is_directory(pathGlobal / item.key())) {
				std::filesystem::remove_all(pathGlobal / item.key());
			}
			else {
				std::filesystem::remove(pathGlobal / item.key());
			}
			Server(ServerCommand::DELETE_FILE, item.key(), "");
			Metadata metadata;
			std::string pathGlobal = "./StoredServerFiles/";
			pathGlobal += userName;
			metadata.folderMetadata(path.string());
			metadata.outputJson(pathGlobal + "/metadata.json");
			/*std::this_thread::sleep_for(std::chrono::milliseconds(250));*/
		}
	}
}

void Account::polling()
{
	mutex.lock();
	std::string pathGlobal = "./StoredServerFiles/";
	std::string pathLocal = "./StoredFiles/";

	pathGlobal += userName;
	pathLocal += userName;

	Metadata metadata;
	metadata.inputJson(pathGlobal + "/metadata.json");
	syncFolderWithMetadata(pathLocal, metadata);
	refresh();
	mutex.unlock();
}

void Account::showTrash()
{
	qDeleteAll(ui.serverWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

	if (!serverOrTrash)
	{
		ui.serverTrash->setText("Recycle Bin");
		ui.restore->show();
		serverOrTrash = true;
		showContentTrash();
	}

	else
	{
		ui.serverTrash->setText("       Server");
		ui.restore->hide();
		serverOrTrash = false;
		showContentServer();
	}
}

void Account::restore()
{
	int folderNumber = 1;
	if (selectedTrash != "")
	{
		if (std::filesystem::is_directory(trashPath / selectedTrash))
		{
			if (!std::filesystem::exists("./StoredFiles/" + userName + "/" + selectedTrash))
			{
				std::filesystem::create_directory("./StoredFiles/" + userName + "/" + selectedTrash);
				copyDirectoryContents(trashPath / selectedTrash, "./StoredFiles/" + userName + "/" + selectedTrash);
			}
			else
			{
				QMessageBox::warning(this, "Alert!", "This file already exists!");
			}
			
		}
		else
		{
			std::filesystem::copy(trashPath / selectedTrash, "./StoredFiles/" + userName);
		}
		std::filesystem::remove_all((trashPath / selectedTrash));
	}

	


	refresh();
}
void Account::addFile()
{
	QFileDialog* file = new QFileDialog(this);
	QString fileSelected = file->getOpenFileName();
	if (!fileSelected.isNull())
	{
		std::string filePath = fileSelected.toStdString();
		std::string fileName = QFileInfo(fileSelected).fileName().toStdString();
		if (!std::filesystem::exists(pathLocal / fileName))
			std::filesystem::copy(filePath, pathLocal);
		else
		{
			QMessageBox::warning(this, "Alert!", "This file already exists!");
		}
	}
	refresh();
}

void Account::deleteLocal()
{
	if (selected != "")
	{
		if (!std::filesystem::is_directory(pathLocal / selected))
		{
			std::string temp = (pathLocal / selected).string();
			std::size_t found = temp.find("Files");

			if (found != std::string::npos)
			{
				temp.insert(found, "Server");
				std::filesystem::remove_all((pathLocal / selected));
				std::filesystem::remove_all((pathGlobal / selected));

				Server(ServerCommand::DOWNLOAD_FILE, temp, "");
				Metadata metadata;
				std::string pathGlobal = "./StoredServerFiles/";
				pathGlobal += userName;

				copyDirectoryContents(pathGlobal, pathLocal.string());

				metadata.folderMetadata(pathLocal.string());
				metadata.outputJson(pathGlobal + "/metadata.json");
				metadata.outputJson(pathLocal.string() + "/metadata.json");

			}
		}
	}
	refresh();
}

void Account::checkLayout(QWidget* currentWidget)
{
	QLayout* layout = currentWidget->layout();

	if (layout != 0)
	{
		QLayoutItem* item;
		while ((item = layout->takeAt(0)) != 0)
			layout->removeItem(item);
		delete layout;
	}
}

QPixmap Account::gridLine(QPixmap pixmap, QLabel* image, QGridLayout* grid, QPushButton* label, uint16_t contorServerGrid, std::filesystem::directory_entry file)
{
	pixmap = pixmap.scaled(image->size(), Qt::KeepAspectRatio);
	image->setPixmap(pixmap);

	QString labelText = QString::fromStdString(file.path().filename().string());
	label->setText(labelText);
	image->setVisible(true);


	grid->addWidget(image, contorServerGrid, 0);
	grid->addWidget(label, contorServerGrid, 1);

	return pixmap;
}

QPixmap Account::gridLayout(QPixmap pixmap, QLabel* image, QGridLayout* grid, QPushButton* label, uint16_t contorServerGrid, std::filesystem::directory_entry file)
{
	pixmap = pixmap.scaled(image->size(), Qt::KeepAspectRatio);
	image->setPixmap(pixmap);
	image->setVisible(true);

	grid->addWidget(image, contorServerGrid, 0);
	QString labelText = QString::fromStdString(file.path().filename().string());

	label->setText(labelText);
	grid->addWidget(label, contorServerGrid, 1);

	return pixmap;
}

bool Account::checkRezervedFiles(std::filesystem::directory_entry file)
{
	if (std::filesystem::is_directory(file))
		if (file.path().filename().string() == "Recycle Bin")
			return true;

	if (file.path().filename().string() == "metadata.json")
		return true;
	return false;
}

void Account::showContentLocal()
{
	labelToBeDeselected = nullptr;
	QPixmap pix;
	checkLayout(ui.localWidget);
	QGridLayout* gridLocal = new  QGridLayout();
	ui.localWidget->setLayout(gridLocal);
	uint16_t contorServerGrid = -1;

	for (const auto& file : std::filesystem::directory_iterator(pathLocal))
	{
		contorServerGrid++;

		QLabel* image = new QLabel(ui.localWidget);
		image->setMinimumSize(QSize(40, 40));

		QPushButton* label = new QPushButton(ui.localWidget);

		label->setMinimumSize(QSize(50, 10));
		label->setVisible(true);
		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		connect(label, &QPushButton::released, this, [=]()
			{
				if (selected == file.path().filename().string() && std::filesystem::is_directory(file))
				{
					selected = "";
					delete gridLocal;
					pathLocal += "/" + file.path().filename().string();
					refreshLocal();
				}
				else
				{
					if (labelToBeDeselected != nullptr)
					{
						labelToBeDeselected->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0);color:black; }");
					}
					labelToBeDeselected = label;
					selected = file.path().filename().string();
					label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0);color:blue; }");
				}
			});

		if (!checkRezervedFiles(file))
			if (std::filesystem::is_directory(file))
			{
				QString filename = "./Assets/FolderIcon.png";

				if (pix.load(filename))
				{
					pix = gridLayout(pix, image, gridLocal, label, contorServerGrid, file);
				}
			}
			else
			{
				if (file.path().filename().string().find(".txt") != std::string::npos)
				{
					QString filename = "./Assets/FileIcon.png";


					if (pix.load(filename))
					{
						pix = gridLine(pix, image, gridLocal, label, contorServerGrid, file);
					}
				}
				else
					if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos)
					{
						QString filename = "./Assets/ImageIcon.jpg";
						if (pix.load(filename))
						{
							pix = gridLine(pix, image, gridLocal, label, contorServerGrid, file);
						}

					}
					else
						if (file.path().filename().string().find(".mp4") != std::string::npos)
						{
							QString filename = "./Assets/MovieIcon.png";
							if (pix.load(filename))
							{
								pix = gridLine(pix, image, gridLocal, label, contorServerGrid, file);
							}
						}
						else
						{
							QString filename = "./Assets/UndefinedIcon.png";
							if (pix.load(filename))
							{
								pix = gridLine(pix, image, gridLocal, label, contorServerGrid, file);
							}
						}
			}

	}

	if (contorServerGrid < iconsOnPage)
	{
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		for (int index = 0; index < iconsOnPage; index++)
		{
			gridLocal->addWidget(spaceExpand, index, 0);
			gridLocal->addWidget(spaceExpand, index, 1);
		}
	}
	ui.localWidget->setVisible(true);
	ui.localFolder->setWidgetResizable(true);
}

void Account::showContentServer()
{
	checkLayout(ui.serverWidget);

	QPixmap pix;
	QGridLayout* gridServer = new QGridLayout();
	ui.serverWidget->setLayout(gridServer);
	uint16_t contorServerGrid = -1;

	for (const auto& file : std::filesystem::directory_iterator(pathGlobal))
	{
		contorServerGrid++;

		QLabel* image = new QLabel(ui.serverWidget);
		image->setMinimumSize(QSize(40, 40));

		QPushButton* label = new QPushButton(ui.serverWidget);
		label->setMinimumSize(QSize(10, 10));
		label->setVisible(true);
		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		if (!checkRezervedFiles(file))
			if (std::filesystem::is_directory(file)) {
				QString filename = "./Assets/FolderIcon.png";

				connect(label, &QPushButton::released, this, [=]()
					{ delete gridServer;
				pathGlobal += "/" + file.path().filename().string();
				refreshServer(); });

				if (pix.load(filename))
				{
					pix = gridLayout(pix, image, gridServer, label, contorServerGrid, file);
				}
			}
			else
			{
				if (file.path().filename().string().find(".txt") != std::string::npos) {
					QString filename = "./Assets/FileIcon.png";

					if (pix.load(filename))
					{
						pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);
					}
				}
				else
					if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos) {
						QString filename = "./Assets/ImageIcon.jpg";
						if (pix.load(filename))
						{
							pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);
						}

					}
					else
						if (file.path().filename().string().find(".mp4") != std::string::npos) {
							QString filename = "./Assets/MovieIcon.png";
							if (pix.load(filename))
							{
								pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);
							}
						}
						else
						{
							QString filename = "./Assets/UndefinedIcon.png";
							if (pix.load(filename))
							{
								pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);

							}
						}
			}

	}
	if (contorServerGrid < iconsOnPage)
	{
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		for (uint16_t index = 0; index < iconsOnPage; index++)
		{
			gridServer->addWidget(spaceExpand, index, 0);
			gridServer->addWidget(spaceExpand, index, 1);
		}
	}

	ui.serverWidget->setVisible(true);
	ui.serverFolder->setWidgetResizable(true);
}

void Account::showContentTrash()
{
	checkLayout(ui.serverWidget);
	labelToBeDeselectedTrash = nullptr;
	QPixmap pix;
	QGridLayout* gridServer = new QGridLayout();
	ui.serverWidget->setLayout(gridServer);
	uint16_t contorServerGrid = -1;

	for (const auto& file : std::filesystem::directory_iterator(trashPath))
	{
		contorServerGrid++;

		QLabel* image = new QLabel(ui.serverWidget);
		image->setMinimumSize(QSize(40, 40));

		QPushButton* label = new QPushButton(ui.serverWidget);
		label->setMinimumSize(QSize(10, 10));
		label->setVisible(true);
		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		connect(label, &QPushButton::released, this, [=]()
			{

				if (selectedTrash == file.path().filename().string() && std::filesystem::is_directory(file))
				{
					selectedTrash = "";
					delete gridServer;
					trashPath += "/" + file.path().filename().string();
					refreshTrash();
				}
				else
				{
					if (labelToBeDeselectedTrash != nullptr)
					{
						labelToBeDeselectedTrash->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0);color:black; }");
					}
					labelToBeDeselectedTrash = label;
					selectedTrash = file.path().filename().string();
					label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0);color:blue; }");
				}});
		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";



			if (pix.load(filename))
			{
				pix = gridLayout(pix, image, gridServer, label, contorServerGrid, file);
			}
		}
		else
		{
			if (file.path().filename().string().find(".txt") != std::string::npos) {
				QString filename = "./Assets/FileIcon.png";

				if (pix.load(filename))
				{
					pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);
				}
			}
			else
				if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos) {
					QString filename = "./Assets/ImageIcon.jpg";
					if (pix.load(filename))
					{
						pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);
					}

				}
				else
					if (file.path().filename().string().find(".mp4") != std::string::npos) {
						QString filename = "./Assets/MovieIcon.png";
						if (pix.load(filename))
						{
							pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);
						}
					}
					else
					{
						QString filename = "./Assets/UndefinedIcon.png";
						if (pix.load(filename))
						{
							pix = gridLine(pix, image, gridServer, label, contorServerGrid, file);

						}
					}
		}

	}
	if (contorServerGrid < iconsOnPage)
	{
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		for (uint16_t index = 0; index < iconsOnPage; index++)
		{
			gridServer->addWidget(spaceExpand, index, 0);
			gridServer->addWidget(spaceExpand, index, 1);
		}
	}
	ui.serverWidget->setVisible(true);
	ui.serverFolder->setWidgetResizable(true);
}
void Account::checkTrash()
{
	if (!std::filesystem::exists(pathLocal / "Recycle Bin")) {
		std::filesystem::create_directory(pathLocal / "Recycle Bin");
	}
}
void Account::startup()
{
	ui.localFolder->setWidget(ui.localWidget);
	ui.serverFolder->setWidget(ui.serverWidget);
	ui.restore->hide();
	ui.serverTrash->setText("       Server");

	std::string userCopy = userName;
	std::transform(userCopy.begin(), userCopy.begin() + 1, userCopy.begin(), ::toupper);
	std::string helloUser = "Hello " + userCopy + "!";

	QString helloUsername = QString::fromStdString(helloUser);
	ui.helloUsername->setText(helloUsername);

	connect(this, SIGNAL(renameFileSignal(std::string&)), this, SLOT(renameFileSlot(std::string&)));
	connect(this, SIGNAL(deleteFileSignal(std::string&)), this, SLOT(deleteFileSlot(std::string&)));
	checkTrash();

	connect(&pollingVariable, SIGNAL(poolingSignal()), this, SLOT(polling()));
	pollingVariable.start();
}
void Account::Server(ServerCommand command, std::string source = "", std::string destination = "")
{
	Client client;
	client.connectServer();
	client.sendUser(client.getSock(), &userName[0]);
	client.sendUser(client.getSock(), &ServerCommandTypeToString(command)[0]);
	
	switch (command)
	{
	case ServerCommand::DELETE_FILE:
		deleteFile(client, source);
		break;
	case ServerCommand::DOWNLOAD_FILE:
		downloadFile(client, source);
		break;
	case ServerCommand::UPDATE_FILE:
		updateFile(client, source);
		break;
	case ServerCommand::RENAME_FILE:
		renameFile(client, source, destination);
		break;
	default:
		break;
	}

	closesocket(client.getSock());
	WSACleanup();
}
Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	selectedTrash = "";
	selected = "";

	this->userName = userName;

	pathLocal = "./StoredFiles/" + userName;
	pathGlobal = "./StoredServerFiles/" + userName;
	trashPath = "./StoredFiles/" + userName + "/Recycle Bin";

	serverOrTrash = false;
	if (!std::filesystem::exists(pathLocal)) 
	{
		std::filesystem::create_directory(pathLocal);
	}

	if (!std::filesystem::exists(pathGlobal)) 
	{
		std::filesystem::create_directory(pathGlobal);
	}

	ui.setupUi(this);
	showContentLocal();
	showContentServer();
	startup();
}
std::string Account::getUser()
{
	return userName;
}
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


void Account::download()
{
	Server("download");
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
		if (pathAuxTrash == ("./StoredFiles/"+userName+"/Recycle Bin"))
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

void Account::renameFileSlot(std::string selected)
{
	if (selected != "")
	{
		bool ok;
		QString text = QInputDialog::getText(this, tr("New name"),
			tr("Please insert a new name:"), QLineEdit::Normal,
			QDir::home().dirName(), &ok);

		if (ok && !text.isEmpty())
		{
			bool found = false;
			for (auto& file : std::filesystem::directory_iterator(pathLocal))
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


					for (int position = selected.find_last_of('.'); position < selected.length(); position++)
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
	QString folder;
	folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	std::filesystem::path filePath = folder.toStdString();

	std::filesystem::create_directory("./StoredFiles/" + userName + "/" + filePath.filename().string());
	copyDirectoryContents(filePath , "./StoredFiles/" + userName + "/" + filePath.filename().string());
	

	
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

void Account::refreshTrash()
{
	qDeleteAll(ui.serverWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	showContentTrash();
}

void Account::deleteFileSlot(std::string selected)
{
	if (selected != "")
	{
		if (std::filesystem::is_directory(pathLocal / selected))
		{
			std::filesystem::create_directory(trashPath / selected);
			copyDirectoryContents(pathLocal / selected, trashPath / selected);
		}
		else
		{
			std::filesystem::copy(pathLocal / selected, trashPath);
		}
		std::filesystem::remove_all((pathLocal / selected));
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
void syncFolderWithMetadata(const std::filesystem::path& path, const Metadata& metadata) {
	FowlerNollVo hashFunction;
	std::unordered_set<long long> hashes;
	for (const auto& it : std::filesystem::directory_iterator(path)) {
		//daca exista fisierul
		if (metadata.m_body.find(it.path().string()) != metadata.m_body.end()) {
			long long lastWriteTime = std::chrono::duration_cast<std::chrono::milliseconds>(it.last_write_time().time_since_epoch()).count();
			if (lastWriteTime > metadata.m_body[it.path().string()]["lastWriteTime"]) {
				std::cout << it.path().string() << ": trebuie actualizat" << std::endl;
			}
		}
		else {//cazul de redenumire
			int rename = 0;
			std::string renamed;
			for (const auto& item : metadata.m_body.items()) {
				if (std::filesystem::is_directory(it.path())) {
					size_t hash = hashFunction.getHashOfFolder(it.path().string());
					if (item.value()["hash"] == hash) {
						rename = 1;
						hashes.insert(hash);
						renamed = item.key();
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
							break;
						}
					}

			}
			if (rename) {
				std::cout << renamed << " fisier redenumit in: " << it.path().string() << std::endl;
			}
			else {//nu exista pana acum
				std::cout << "fisier/folder nou: " << it.path().string() << std::endl;

			}
		}
	}
	//cazul de stergere
	for (const auto& item : metadata.m_body.items()) {
		int name = 1, hash = 1;
		if (!std::filesystem::exists(item.key())) {
			name = 0;
		}
		if (hashes.find(item.value()["hash"]) == hashes.end()) {
			hash = 0;
		}
		if (name == 0 && hash == 0) {
			std::cout << item.key() << " a fost sters" << std::endl;
		}
	}
}

void Account::polling()
{
	std::string pathGlobal = "./StoredServerFiles/";
	std::string pathLocal = "./StoredFiles/";
	pathGlobal += userName;
	pathLocal += userName;
	Metadata metadata;
	metadata.inputJson(pathGlobal+ "/metadata.json");
	syncFolderWithMetadata(pathLocal, metadata);
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
	if (selectedTrash != "")
	{
		if (std::filesystem::is_directory(trashPath / selectedTrash))
		{
			std::filesystem::create_directory("./StoredFiles/" + userName+"/"+ selectedTrash);
			copyDirectoryContents(trashPath / selectedTrash, "./StoredFiles/" + userName+ "/" + selectedTrash);
			
		}
		else
		{
			std::filesystem::copy(trashPath / selectedTrash, "./StoredFiles/" + userName);
		}
		std::filesystem::remove_all((trashPath / selectedTrash));
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

QPixmap Account::gridLine(QPixmap pixmap, QLabel* image, QGridLayout* grid, QPushButton* label, int contorServerGrid, std::filesystem::directory_entry file)
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

QPixmap Account::gridLayout(QPixmap pixmap, QLabel* image, QGridLayout* grid, QPushButton* label, int contorServerGrid, std::filesystem::directory_entry file)
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
	int contorServerGrid = -1;

	for (auto& file : std::filesystem::directory_iterator(pathLocal))
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
	if (contorServerGrid < 7)

	{
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		for (int index = 0; index < 7; index++)
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
	int contorServerGrid = -1;

	for (auto& file : std::filesystem::directory_iterator(pathGlobal))
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
	if (contorServerGrid < 7)
	{
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		for (int index = 0; index < 7; index++)
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
	int contorServerGrid = -1;

	for (auto& file : std::filesystem::directory_iterator(trashPath))
	{

		contorServerGrid++;

		QLabel* image = new QLabel(ui.serverWidget);
		image->setMinimumSize(QSize(40, 40));

		QPushButton* label = new QPushButton(ui.serverWidget);
		label->setMinimumSize(QSize(10, 10));
		label->setVisible(true);
		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";

			connect(label, &QPushButton::released, this, [=]()
				{ 
					std::cout << "in";
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
	if (contorServerGrid < 7)
	{
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		for (int index = 0; index < 7; index++)
		{
			gridServer->addWidget(spaceExpand, index, 0);
			gridServer->addWidget(spaceExpand, index, 1);
		}
	}
	ui.serverWidget->setVisible(true);
	ui.serverFolder->setWidgetResizable(true);
}
void Account::Server(std::string command) {
	Client client;
	std::string temp = getUser();
	char* username = &temp[0];
	std::string path = "./StoredServerFiles";
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	path += "/";
	path += username;
	client.connectServer();
	client.sendUser(client.getSock(), username);
	client.sendUser(client.getSock(), &command[0]);
	char fileName[FILENAME_MAX];
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	int ok;
	do {
		char fileName[FILENAME_MAX];
		ok = 0;
		char fileType[FILENAME_MAX];
		strcpy(fileName, client.recvFileName(client.getSock()));
		if (fileName == "/eof" || fileType == "eof")
			ok = 1;
		if (ok != 1)
		{
			strcpy(fileType, client.recvFileName(client.getSock()));
			std::cout << fileName << std::endl << fileType << std::endl;
			if (strstr(fileType, "folder"))
			{
				std::filesystem::create_directory(fileName);
			}
			else
			{
				long size = client.recvFileSize(client.getSock());
				if (size > 0)
				{

					if (client.writeToFile(client.getSock(), fileName, size) == 0)
						ok = 1;

				}
				else if (size == 0)
				{
					std::ofstream file;
					file.open(fileName);
				}
				else if (size == -1)
					ok = 1;
			}
		}
	} while (ok != 1);
	int flush = 0;
	char fileRequested[FILENAME_MAX];
	do
	{
		flush = recv(client.getSock(), fileRequested, FILENAME_MAX, 0);
		std::cout << flush << std::endl;
	} while (flush > 0);
	closesocket(client.getSock());
	WSACleanup();
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
	connect(this, SIGNAL(renameFileSignal(std::string)), this, SLOT(renameFileSlot(std::string)));
	connect(this, SIGNAL(deleteFileSignal(std::string)), this, SLOT(deleteFileSlot(std::string)));
	checkTrash();

	connect(&pollingVariable, SIGNAL(poolingSignal()), this, SLOT(polling()));
	pollingVariable.start();
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
	if (!std::filesystem::exists(pathLocal)) {
		std::filesystem::create_directory(pathLocal);
	}
	if (!std::filesystem::exists(pathGlobal)) {
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
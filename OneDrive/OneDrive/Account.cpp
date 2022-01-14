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
	std::string path_aux = pathGlobal.string();
	if (path_aux == ("./StoredServerFiles/" + userName))
	{
		//throw "Back out of the restricted area";
		return;
	}
	while (path_aux[path_aux.length() - 1] != '/')
	{
		path_aux.erase(path_aux.begin() + path_aux.length() - 1);
	}
	path_aux.erase(path_aux.begin() + path_aux.length() - 1);
	pathGlobal = path_aux;
	refreshServer();
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



}

void Account::addFolder()
{
	QDialog* fileName = new QFileDialog;
	fileName->show();
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

void Account::deleteFileSlot(std::string selected)
{
	if (selected != "")
		std::filesystem::remove_all((pathLocal / selected));
	else
		QMessageBox::warning(this, "Alert!", "Please select a file to delete!");
	refreshLocal();
}

void Account::refresh()
{
	refreshLocal();
	refreshServer();
}

void Account::polling()
{
	{
		std::cout << "Thread \n";
	}
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

Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	selected = "";
	this->userName = userName;
	pathLocal = "./StoredFiles/" + userName;
	pathGlobal = "./StoredServerFiles/" + userName;
	if (!std::filesystem::exists(pathLocal)) {
		std::filesystem::create_directory(pathLocal);
	}
	if (!std::filesystem::exists(pathGlobal)) {
		std::filesystem::create_directory(pathGlobal);
	}
	ui.setupUi(this);
	showContentLocal();
	showContentServer();
	ui.localFolder->setWidget(ui.localWidget);
	ui.serverFolder->setWidget(ui.serverWidget);
	connect(this, SIGNAL(renameFileSignal(std::string)), this, SLOT(renameFileSlot(std::string)));
	connect(this, SIGNAL(deleteFileSignal(std::string)), this, SLOT(deleteFileSlot(std::string)));

	connect(&pollingVariable, SIGNAL(poolingSignal()), this, SLOT(polling()));

	pollingVariable.start();
}

std::string Account::getUser()
{

	return userName;

}



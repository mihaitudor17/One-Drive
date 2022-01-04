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
	downloadServer();
}

void Account::back_folder_local()
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
	qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	pathLocal = path_aux;
	showContentLocal();
}

void Account::back_folder_server()
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
	qDeleteAll(ui.serverWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	pathGlobal = path_aux;
	showContentServer();
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
					qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
					showContentLocal();
				}
				else
				{
					std::string extension;
					
					
					for (int position = selected.find_last_of('.'); position < selected.length(); position++)
					{
						extension.push_back(selected[position]);
					}
					std::filesystem::rename(pathLocal / selected, pathLocal/(text.toStdString()+extension));
					qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
					showContentLocal();
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

void Account::deleteFileSlot(std::string selected)
{
	if (selected != "")
		std::filesystem::remove_all((pathLocal / selected));
	else
		QMessageBox::warning(this, "Alert!", "Please select a file to delete!");
	qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
	showContentLocal();
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
					qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
					showContentLocal();
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

		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";

			if (pix.load(filename)) {

				pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
				image->setPixmap(pix);
				image->setVisible(true);
				gridLocal->addWidget(image, contorServerGrid, 0);
				QString labelText = QString::fromStdString(file.path().filename().string());
				label->setText(labelText);
				gridLocal->addWidget(label, contorServerGrid, 1);

			}
		}
		else {
			if (file.path().filename().string().find(".txt") != std::string::npos) {
				QString filename = "./Assets/FileIcon.png";


				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridLocal->addWidget(image, contorServerGrid, 0);
					gridLocal->addWidget(label, contorServerGrid, 1);

				}
			}
			else if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos) {
				QString filename = "./Assets/ImageIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridLocal->addWidget(image, contorServerGrid, 0);
					gridLocal->addWidget(label, contorServerGrid, 1);

				}

			}
			else if (file.path().filename().string().find(".mp4") != std::string::npos) {
				QString filename = "./Assets/MovieIcon.png";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridLocal->addWidget(image, contorServerGrid, 0);
					gridLocal->addWidget(label, contorServerGrid, 1);

				}
			}
			else {
				QString filename = "./Assets/UndefinedIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridLocal->addWidget(image, contorServerGrid, 0);
					gridLocal->addWidget(label, contorServerGrid, 1);

				}
			}
		}

	}
	if (contorServerGrid < 3)

	{
		if (contorServerGrid == -1)
			contorServerGrid++;
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		gridLocal->addWidget(spaceExpand, contorServerGrid, 0);
		gridLocal->addWidget(spaceExpand, contorServerGrid, 1);
		gridLocal->addWidget(spaceExpand, contorServerGrid + 1, 0);
		gridLocal->addWidget(spaceExpand, contorServerGrid + 1, 1);
		gridLocal->addWidget(spaceExpand, contorServerGrid + 2, 0);
		gridLocal->addWidget(spaceExpand, contorServerGrid + 2, 1);
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
			qDeleteAll(ui.serverWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
			showContentServer(); });

			if (pix.load(filename)) {

				pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
				image->setPixmap(pix);
				image->setVisible(true);
				gridServer->addWidget(image, contorServerGrid, 0);
				QString labelText = QString::fromStdString(file.path().filename().string());
				label->setText(labelText);
				gridServer->addWidget(label, contorServerGrid, 1);

			}
		}
		else {
			if (file.path().filename().string().find(".txt") != std::string::npos) {
				QString filename = "./Assets/FileIcon.png";


				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridServer->addWidget(image, contorServerGrid, 0);
					gridServer->addWidget(label, contorServerGrid, 1);

				}
			}
			else if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos) {
				QString filename = "./Assets/ImageIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridServer->addWidget(image, contorServerGrid, 0);
					gridServer->addWidget(label, contorServerGrid, 1);

				}

			}
			else if (file.path().filename().string().find(".mp4") != std::string::npos) {
				QString filename = "./Assets/MovieIcon.png";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridServer->addWidget(image, contorServerGrid, 0);
					gridServer->addWidget(label, contorServerGrid, 1);

				}
			}
			else {
				QString filename = "./Assets/UndefinedIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					image->setVisible(true);
					gridServer->addWidget(image, contorServerGrid, 0);
					gridServer->addWidget(label, contorServerGrid, 1);

				}
			}
		}

	}
	if (contorServerGrid < 3)

	{
		if (contorServerGrid == -1)
			contorServerGrid++;
		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		gridServer->addWidget(spaceExpand, contorServerGrid, 0);
		gridServer->addWidget(spaceExpand, contorServerGrid, 1);
		gridServer->addWidget(spaceExpand, contorServerGrid + 1, 0);
		gridServer->addWidget(spaceExpand, contorServerGrid + 1, 1);
		gridServer->addWidget(spaceExpand, contorServerGrid + 2, 0);
		gridServer->addWidget(spaceExpand, contorServerGrid + 2, 1);
	}
	ui.serverWidget->setVisible(true);
	ui.serverFolder->setWidgetResizable(true);

}
void Account::downloadServer() {
	Client client;
	char* username = "Tinel";//De obtinut userul  //crapa pt ca nu gaseste calea
	std::string path = "./Stored Files";
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	path += "/";
	path += username;
	client.connectServer();
	client.sendUser(client.getSock(), username);
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
		strcpy(fileType, client.recvFileName(client.getSock()));
		std::cout << fileName << std::endl << fileType << std::endl;
		if (strstr(fileType, "folder"))
		{
			std::filesystem::create_directory(fileName);
		}
		else
		{
			long size = client.recvFileSize(client.getSock());
			if (size != 0)
			{

				if (client.writeToFile(client.getSock(), fileName, size) == 0)
					ok = 1;
			}
			else
			{
				std::ofstream file;
				file.open(fileName);
			}
		}
	} while (ok != 1);
}
Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	selected = "";
	this->userName = userName;
	pathLocal = "./StoredFiles/" + userName;
	pathGlobal = "./StoredServerFiles/" + userName;
	ui.setupUi(this);
	showContentLocal();
	showContentServer();
	ui.localFolder->setWidget(ui.localWidget);
	ui.serverFolder->setWidget(ui.serverWidget);
	connect(this, SIGNAL(renameFileSignal(std::string)), this, SLOT(renameFileSlot(std::string)));
	connect(this, SIGNAL(deleteFileSignal(std::string)), this, SLOT(deleteFileSlot(std::string)));
	//turc was here
}

std::string Account::getUser()
{
	
		return userName;
	
}



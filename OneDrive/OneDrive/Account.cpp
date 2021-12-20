#include "Account.h"
#include<QPushButton>
#include <iostream>
#include <QVBoxLayout>
#include <QGridLayout>




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
	std::cout << selected;
}

void Account::renameLocalSendSignal()
{
	emit renameFileSignal(selected);
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

		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";

			connect(label, &QPushButton::released, this, [=]()
				{ 
					if (selected==file.path().filename().string())
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
						selected  = file.path().filename().string();
						label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0);color:blue; }");
					}
						
			
				});

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


Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	selected = "assdf";
	this->userName = userName;
	pathLocal = "./StoredFiles/" + userName;
	pathGlobal = "./StoredServerFiles/" + userName;
	ui.setupUi(this);
	showContentLocal();
	showContentServer();
	ui.localFolder->setWidget(ui.localWidget);
	ui.serverFolder->setWidget(ui.serverWidget);
	connect(this, SIGNAL(renameFileSignal(std::string)), this, SLOT(renameFileSlot(std::string)));
	

}



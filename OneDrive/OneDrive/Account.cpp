#include "Account.h"
#include<QPushButton>
#include <iostream>
#include <QVBoxLayout>
#include <QGridLayout>


void Account::showContentLocal()
{
	QPixmap pix;
	QGridLayout* grid = new QGridLayout();
	ui.localWidget->setLayout(grid);
	int contorLocalGrid = -1;

	for (auto& file : std::filesystem::directory_iterator(pathLocal))
	{

		contorLocalGrid++;

		QLabel* image = new QLabel(ui.localWidget);
		image->setMinimumSize(QSize(40, 40));

		QPushButton* label = new QPushButton(ui.localWidget);
		label->setMinimumSize(QSize(10, 10));
		label->setVisible(true);
		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";

			connect(label, &QPushButton::released, this, [=]()
				{ delete grid;
			pathLocal += "/" + file.path().filename().string();
			qDeleteAll(ui.localWidget->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
			showContentLocal(); });

			if (pix.load(filename)) {

				pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
				image->setPixmap(pix);
				image->setVisible(true);
				grid->addWidget(image, contorLocalGrid, 0);
				QString labelText = QString::fromStdString(file.path().filename().string());
				label->setText(labelText);
				grid->addWidget(label, contorLocalGrid, 1);

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
					grid->addWidget(image, contorLocalGrid, 0);
					grid->addWidget(label, contorLocalGrid, 1);

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
					grid->addWidget(image, contorLocalGrid, 0);
					grid->addWidget(label, contorLocalGrid, 1);

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
					grid->addWidget(image, contorLocalGrid, 0);
					grid->addWidget(label, contorLocalGrid, 1);

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
					grid->addWidget(image, contorLocalGrid, 0);
					grid->addWidget(label, contorLocalGrid, 1);

				}
			}
		}

	}
	if (contorLocalGrid < 3)

	{

		QWidget* spaceExpand = new QWidget();
		spaceExpand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		grid->addWidget(spaceExpand, contorLocalGrid, 0);
		grid->addWidget(spaceExpand, contorLocalGrid, 1);
		grid->addWidget(spaceExpand, contorLocalGrid + 1, 0);
		grid->addWidget(spaceExpand, contorLocalGrid + 1, 1);
		grid->addWidget(spaceExpand, contorLocalGrid + 2, 0);
		grid->addWidget(spaceExpand, contorLocalGrid + 2, 1);
	}
	ui.localWidget->setVisible(true);
	ui.localFolder->setWidgetResizable(true);




}

void Account::showContentServer()
{
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
	this->userName = userName;
	pathLocal = "./StoredFiles/" + userName;
	pathGlobal = "./StoredServerFiles/" + userName;
	ui.setupUi(this);
	showContentLocal();
	showContentServer();
	ui.localFolder->setWidget(ui.localWidget);
	ui.serverFolder->setWidget(ui.serverWidget);

}



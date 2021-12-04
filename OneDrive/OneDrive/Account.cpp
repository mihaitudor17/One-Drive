#include "Account.h"
#include<QPushButton>
#include <iostream>
#include <QVBoxLayout>
#include <QGridLayout>


void Account::showContentsOfFile()
{
	int widthAdjust = -30;
	QPixmap pix;
	QGridLayout* grid = new QGridLayout(ui.test);
	int i = -1;

	for (auto& file : std::filesystem::directory_iterator(pathLocal))
	{

		i++;
		widthAdjust += 30;
		QLabel* image = new QLabel(ui.test);
		image->setMinimumSize(QSize(40, 40));
		int coordinatesX = image->x();
		int coordinatesY = image->y();
		int coordinatesWidth = image->width();
		int coordinatesHeight = image->height();
		QPushButton* label = new QPushButton(ui.test);
		label->setMinimumSize(QSize(10, 10));

		label->setVisible(true);

		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";

			connect(label, &QPushButton::released, this, [=]()
				{ pathLocal += "/" + file.path().filename().string();
			qDeleteAll(ui.folderLocal->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
			showContentsOfFile(); });

			if (pix.load(filename)) {

				pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
				image->setPixmap(pix);
				//image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
				image->setVisible(true);
				grid->addWidget(image, i, 0);
				QString labelText = QString::fromStdString(file.path().filename().string());
				label->setText(labelText);
				//label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
				grid->addWidget(label, i, 1);

			}
		}
		else {
			if (file.path().filename().string().find(".txt") != std::string::npos) {
				QString filename = "./Assets/FileIcon.png";


				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					//image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					//label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);
					grid->addWidget(image, i, 0);
					grid->addWidget(label, i, 1);

				}
			}
			else if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos) {
				QString filename = "./Assets/ImageIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					//image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					//label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);
					grid->addWidget(image, i, 0);
					grid->addWidget(label, i, 1);

				}

			}
			else if (file.path().filename().string().find(".mp4") != std::string::npos) {
				std::cout << "Crig";
				QString filename = "./Assets/MovieIcon.png";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					//image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					//label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);
					grid->addWidget(image, i, 0);
					grid->addWidget(label, i, 1);

				}
			}
			else {
				QString filename = "./Assets/UndefinedIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					//image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					//label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);
					grid->addWidget(image, i, 0);
					grid->addWidget(label, i, 1);

				}
			}
		}

	}

	ui.test->setVisible(true);
	ui.folderLocal->setWidgetResizable(true);
	ui.folderLocal->setWidget(ui.test);



}


Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	this->userName = userName;
	pathLocal = "./StoredFiles/" + userName;
	ui.setupUi(this);
	showContentsOfFile();

}



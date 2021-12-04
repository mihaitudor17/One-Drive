#include "Account.h"
#include<QPushButton>
#include <iostream>

void Account::openFile()
{



}


void Account::showContentsOfFile()
{
	std::cout << pathLocal;
	int widthAdjust = -30;
	QPixmap pix;


	for (auto& file : std::filesystem::directory_iterator(pathLocal))
	{

		widthAdjust += 30;
		QLabel* image = new QLabel(ui.frame);
		int coordinatesX = image->x();
		int coordinatesY = image->y();
		int coordinatesWidth = image->width();
		int coordinatesHeight = image->height();
		QPushButton* label = new QPushButton(ui.frame);
		label->setVisible(true);

		label->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");

		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";

			connect(label, &QPushButton::released, this, [=]() { pathLocal += "/" + file.path().filename().string();
			qDeleteAll(ui.frame->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
			showContentsOfFile(); });

			if (pix.load(filename)) {

				pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
				image->setPixmap(pix);
				image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
				QString labelText = QString::fromStdString(file.path().filename().string());
				label->setText(labelText);
				label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
				image->setVisible(true);
			}
		}
		else {
			if (file.path().filename().string().find(".txt") != std::string::npos) {
				QString filename = "./Assets/FileIcon.png";


				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);

				}
			}
			else if (file.path().filename().string().find(".jpg") != std::string::npos || file.path().filename().string().find(".png") != std::string::npos) {
				QString filename = "./Assets/ImageIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);

				}

			}
			else if (file.path().filename().string().find(".mp4") != std::string::npos) {
				std::cout << "Crig";
				QString filename = "./Assets/MovieIcon.png";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);

				}
			}
			else {
				QString filename = "./Assets/UndefinedIcon.jpg";
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					image->setVisible(true);

				}
			}
		}

	}
}


Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	this->userName = userName;
	pathLocal = "./StoredFiles/" + userName;
	ui.setupUi(this);
	showContentsOfFile();

}



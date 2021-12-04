#include "Account.h"
#include <iostream>
void Account::openFile()
{
	this->setStyleSheet("color:blue;");

}
void Account::showContentsOfFile()
{

	int widthAdjust = -30;
	QPixmap pix;
	std::filesystem::path path = "./StoredFiles/" + userName;

	for (auto& file : std::filesystem::directory_iterator(path))
	{
		widthAdjust += 30;
		QLabel* image = new QLabel(ui.frame);
		int coordinatesX = image->x();
		int coordinatesY = image->y();
		int coordinatesWidth = image->width();
		int coordinatesHeight = image->height();
		QLabel* label = new QLabel(ui.frame);
		if (std::filesystem::is_directory(file)) {
			QString filename = "./Assets/FolderIcon.png";
			if (pix.load(filename)) {

				pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
				image->setPixmap(pix);
				image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
				QString labelText = QString::fromStdString(file.path().filename().string());
				label->setText(labelText);
				label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
			}
		}
		else {
			if (file.path().filename().string().find(".txt") != std::string::npos) {
				QString filename = "./Assets/FileIcon.jpg";
				connect(label, &QPushButton::released, this, &Account::openFile);
				if (pix.load(filename)) {
					pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
					image->setPixmap(pix);
					image->setGeometry(coordinatesX, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
					QString labelText = QString::fromStdString(file.path().filename().string());
					label->setText(labelText);
					label->setGeometry(coordinatesX + 30, coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);

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

				}
			}
		}

	}
}


Account::Account(const std::string& userName, QWidget* parent)
	: QWidget(parent)
{
	this->userName = userName;
	ui.setupUi(this);
	showContentsOfFile();

}
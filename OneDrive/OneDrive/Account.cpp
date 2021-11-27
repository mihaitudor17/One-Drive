#include "Account.h"
#include <iostream>

void Account::showContentsOfFile()
{
	QString filename = "./folderIcon.png";
	int widthAdjust = -30;
	std::filesystem::path path= "./StoredFiles/"+userName;
	//std::filesystem::path path = "C:/Users/40763/Desktop/OneDrive/OneDrive/OneDrive/StoredFiles/tinel";
	for (auto& file:std::filesystem::directory_iterator(path)) 
	{
		widthAdjust += 30;
		QLabel* image = new QLabel(ui.frame);
		QPixmap pix;
		int coordinatesX = image->x();
		int coordinatesY = image->y();
		int coordinatesWidth = image->width();
		int coordinatesHeight = image->height();
		/** to check wether load ok */
		if (pix.load(filename)) {
			/** scale pixmap to fit in label'size and keep ratio of pixmap */
			pix = pix.scaled(image->size(), Qt::KeepAspectRatio);
			image->setPixmap(pix);
			image->setGeometry(coordinatesX , coordinatesY + widthAdjust, coordinatesWidth, coordinatesHeight);
			
		}

	}
}

Account::Account(const std:: string &userName ,QWidget *parent)
	: QWidget(parent)
{
	this->userName = userName;
	ui.setupUi(this);
	showContentsOfFile();
	
}



#pragma once
#include <QWidget>
#include "ui_Account.h"

class Account : public QWidget
{
	Q_OBJECT

public:
	Account(QWidget *parent = 0);
	

private:
	Ui::Account ui;
};

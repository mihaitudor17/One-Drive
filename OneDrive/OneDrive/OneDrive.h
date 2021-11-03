#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OneDrive.h"

class OneDrive : public QMainWindow
{
    Q_OBJECT

public:
    OneDrive(QWidget *parent = Q_NULLPTR);

private:
    Ui::OneDriveClass ui;
};

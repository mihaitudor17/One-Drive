#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OneDrive.h"
#include <QVBoxLayout>
#include <QLabel>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = 0);





private:
    
    QLabel* labelUsername;
    QLineEdit* editUsername;
    QLabel* label;
};

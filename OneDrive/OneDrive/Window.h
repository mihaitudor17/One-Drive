#pragma once
#include "JSON.h"
#include <QtWidgets/QMainWindow>
#include "ui_OneDrive.h"
#include <QVBoxLayout>
#include <QLabel>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = 0);

    private slots:
    void on_Button_clicked();
    void Signup();


private:
    
    QLabel* labelUsername;
    QLineEdit* editUsername;
    QLabel* label;
};

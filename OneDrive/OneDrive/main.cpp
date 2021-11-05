#include "Window.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <QWidget>
#include <QObject>
#include <QString>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.setFixedSize(700, 700);
    w.show();

    return a.exec();
}

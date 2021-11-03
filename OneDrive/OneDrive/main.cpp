#include "OneDrive.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OneDrive w;
    w.show();
    return a.exec();
}

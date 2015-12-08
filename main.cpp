#include "flashlamp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    flashlamp w;
    w.show();

    return a.exec();
}

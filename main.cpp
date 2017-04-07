#include "ocrmodule.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    LeptonicaOCRModule ocrModule{"pol"};
    MainWindow w{ocrModule};
    w.show();

    return a.exec();
}

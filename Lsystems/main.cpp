#include "mainwindow.h"
#include <QApplication>
#include "lsystem.h"
#include <iostream>
#include <fstream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

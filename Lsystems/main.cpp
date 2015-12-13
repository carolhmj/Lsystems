#include "mainwindow.h"
#include <QApplication>
#include "lsystem.h"
#include <iostream>
#include <fstream>
int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
      Lsystem sys("../teste.txt");
      sys.printRules();
      sys.setAxiom("XAAX");
      for (int i = 0; i < 3; i++){
        sys.evolveState();
      }
}

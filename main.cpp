#include "mainwindow.h"
#include "drug.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QListWidget>
#include <QFile>
#include <QDebug>

#define LOG std::cout<<

std::vector<Drug> DrugList;
std::vector<Drug *> DrugSorted;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::ifstream infile("drugs.txt");
    std::string line;

    while (std::getline(infile, line))
    {
        Drug tmp;
        tmp.loadFromLine(line);
        DrugList.push_back(tmp);
    }

    /*for(int i=0; i<DrugList.size(); ++i)
    {
        qDebug()<<QString::fromStdString(DrugList[i].name)
               <<QString::fromStdString(DrugList[i].test[0].toHex())
                <<QString::fromStdString(DrugList[i].test[1].toHex());
    }*/

    MainWindow w;

    w.setWindowFlags(Qt::WindowStaysOnTopHint);

    w.show();

    return a.exec();
}

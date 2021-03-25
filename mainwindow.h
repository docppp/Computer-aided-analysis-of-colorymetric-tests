#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextBrowser>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QTableWidget>
#include <QCheckBox>
#include <QColorDialog>
#include "color.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int activeDrug  = -1;
    int activeTest  = 0;
    int trackMouse  = 0;
    int colorButton = 0;
    int colorPicker = 0;

    Color colorMid  = Color(-1);
    Color colorStop = Color(-1);

public slots:
    void getItem(QListWidgetItem *it);
    void searchItem();
    void getPixel();
    void getTest(int i);
    void resetColors();
    void updateTable();
    void resetTable();
    void editDataBase();
    void setPicker(int i);

signals:

private:
    Ui::MainWindow  *ui;
    QListWidget     *listWidget;
    QTextBrowser    *textBrowser;
    QTextBrowser    *textTest[13];
    QPainter        *painter;
    QTableWidget    *tableWidget;
    QCheckBox       *useColorPicker;

protected:
    void paintEvent(QPaintEvent *);
    void focusOutEvent(QFocusEvent *e);
};

unsigned int lev_dam_dist(std::string s1,  std::string s2);

void CheckErrors(const Color &sample1, const Color &sample2, int testFlag);

#endif // MAINWINDOW_H

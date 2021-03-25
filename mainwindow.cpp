#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drug.h"
#include <algorithm>
#include <QDebug>
#include <QPixmap>
#include <QDesktopWidget>
#include <QCursor>
#include <fstream>

extern std::vector<Drug> DrugList;
extern std::vector<Drug *> DrugSorted;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textBrowser = new QTextBrowser(this);
    textBrowser->setGeometry(QRect(20, 230, 270, 20));
    textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textBrowser->setReadOnly(false);

    listWidget = new QListWidget(this);
    listWidget->setGeometry(QRect(20, 260, 270, 230));

    QString testNames[NUMBER_OF_TESTS]={"MARQUIS","MECKE",
            "MANDELIN","LIEBERMANN","FROEHDE","ROBADOPE",
            "SIMONS","FOLIN","EHRLICH","HOFMANN","SCOTT",
            "KWAS GALUSOWY","ZIMMERMAN"};

    for (int i = 0; i < NUMBER_OF_TESTS; i++)
    {
        textTest[i] = new QTextBrowser(this);
        textTest[i]->setGeometry(QRect(320, 230+i*20, 270, 20));
        textTest[i]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textTest[i]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textTest[i]->setFontWeight(QFont::Bold);
        textTest[i]->setText(testNames[i]);
        textTest[i]->setAlignment(Qt::AlignCenter);
        QPalette p = palette();
        p.setColor(QPalette::Base, QColor(0,0,0,0));
        textTest[i]->setPalette(p);
        textTest[i]->setReadOnly(true);
    }

    useColorPicker = new QCheckBox(this);
    useColorPicker->setGeometry(QRect(25, 180, 120, 20));
    useColorPicker->setText("Użyj color pickera");

    connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this,SLOT(getItem(QListWidgetItem*)));

    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(repaint()));

    connect(textBrowser,SIGNAL(textChanged()),
            this,SLOT(searchItem()));

    connect(useColorPicker, SIGNAL(stateChanged(int)),
            this,SLOT(setPicker(int)));

    for (int i = 0; i < DrugList.size(); ++i)
    {
        QString s = QString::fromStdString(DrugList[i].name);
        new QListWidgetItem(s, listWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getItem(QListWidgetItem *it)
{
    if (it != 0)
    {
        int i = 0;

        for (; i < DrugList.size(); ++i)
        {
            if (it->text().toStdString()==DrugList[i].name) break;
        }

        activeDrug = i;
    }
}

void MainWindow::searchItem()
{
    int values = listWidget->count();

    for (int i = 0; i < values; ++i)
    {
        QListWidgetItem *item = listWidget->item(0);
        listWidget->removeItemWidget(item);
        delete item;
    }

    QString qtext = textBrowser->toPlainText();

    std::string text = qtext.toStdString();

    std::transform(text.begin(), text.end(), text.begin(), ::tolower);

    if (text.compare("") == 0)
    {
        for (int i = 0; i < DrugList.size(); ++i)
        {
            QString s = QString::fromStdString(DrugList[i].name);
            new QListWidgetItem(s, listWidget);
        }

        return;
    }

    for (int i = 0; i < DrugList.size(); ++i)
    {
        std::string name = DrugList[i].name;

        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (name.find(text) != std::string::npos)
        {
            QString s = QString::fromStdString(DrugList[i].name);
            new QListWidgetItem(s, listWidget);

            continue;
        }

        if (name.compare(text) == 0)
        {
            listWidget->clear();

            QString s = QString::fromStdString(DrugList[i].name);
            new QListWidgetItem(s, listWidget);

            return;
        }

        if (lev_dam_dist(name, text) < 2)
        {
            QString s = QString::fromStdString(DrugList[i].name);
            new QListWidgetItem(s, listWidget);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    //
    // Painting big rectanlge in test search part.
    //

    QRect recBig(20, 100, 275, 30);

    QLinearGradient gradBig(recBig.topLeft(), recBig.bottomRight());

    QColor colorStart = fromHex(testColor(activeTest));

    gradBig.setColorAt(0, colorStart);

    if (colorMid.R != -1)
    {
        QColor QcolorMid(colorMid.R, colorMid.G, colorMid.B);
        gradBig.setColorAt(0.5, QcolorMid);
    }

    if (colorStop.R != -1)
    {
        QColor QcolorStop(colorStop.R, colorStop.G, colorStop.B);
        gradBig.setColorAt(1, QcolorStop);
    }
    else gradBig.setColorAt(1, colorStart);

    painter.fillRect(recBig,gradBig);

    //
    // Paitinig rectangles in model search part.
    //

    if (activeDrug != -1)
        for (int i = 0; i < NUMBER_OF_TESTS; ++i)
    {
        QRect rec(320, 230+i*20, 270, 20);

        QLinearGradient grad(rec.topLeft(), rec.bottomRight());

        Color colorMid  = DrugList[activeDrug].test[i*2];
        Color colorStop = DrugList[activeDrug].test[i*2+1];

        if (colorMid.R == -1 && colorStop.R == -1)
        {
            grad.setColorAt(0, QColor(0,0,0));
            grad.setColorAt(1, QColor(0,0,0));
        }
        else
        {
            grad.setColorAt(0,fromHex(testColor(i)));

            QColor QcolorStop(colorStop.R, colorStop.G, colorStop.B);
            grad.setColorAt(1, QcolorStop);

            if (colorMid.R != -1)
            {
                QColor QcolorMid(colorMid.R, colorMid.G, colorMid.B);
                grad.setColorAt(0.5, QcolorMid);
            }
        }

        painter.fillRect(rec,grad);
    }

    painter.end();
}

void MainWindow::getPixel()
{
    QObject* button = QObject::sender();

    if (button == ui->button_color1) colorButton = 1;
    if (button == ui->button_color2) colorButton = 2;

    if(colorPicker)
    {
        QColor pix = QColorDialog::getColor(Qt::white, this);

        Color tmp = Color(pix.red(), pix.green(), pix.blue());

        if (colorButton == 1) colorMid.copyFrom(tmp);
        if (colorButton == 2) colorStop.copyFrom(tmp);

        colorButton = 0;
        trackMouse  = 0;

        return;
    }

    QApplication::setOverrideCursor(Qt::CrossCursor);

    trackMouse = 1;

    setFocus();
}

void MainWindow::setPicker(int i)
{
    colorPicker = i;
}

void MainWindow::focusOutEvent(QFocusEvent *e)
{
    if (trackMouse)
    {
        QPoint coord = QCursor::pos();

        QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

        QRgb pixelValue = pixmap.toImage().pixel(coord.rx(),coord.ry());

        QColor pix(pixelValue);

        Color tmp = Color(pix.red(), pix.green(), pix.blue());

        if (colorButton == 1) colorMid.copyFrom(tmp);
        if (colorButton == 2) colorStop.copyFrom(tmp);

        colorButton = 0;
        trackMouse  = 0;
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void MainWindow::getTest(int i)
{
    activeTest = i;

    repaint();
}

void MainWindow::resetColors()
{
    colorMid  = Color(-1);
    colorStop = Color(-1);

    repaint();
}

void MainWindow::updateTable()
{
    int isColorSet1 = 1;
    int isColorSet2 = 1;
    if (DrugSorted.size() == 0)
    {
        for (int i = 0; i < DrugList.size(); ++i)
            DrugSorted.push_back(&DrugList[i]);
    }

    if (colorStop.R == -1)
    {
        isColorSet2 = 0;
        colorStop.loadFromHex(testColor(activeTest));
    }

    if (colorMid.R == -1)
    {
        isColorSet1 = 0;
        Color colorStart;
        colorStart.loadFromHex(testColor(activeTest));

        colorMid.inBetween(colorStart, colorStop);
    }

    CheckErrors(colorMid, colorStop, activeTest);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(DrugSorted.size());
    for (int i = 0; i < DrugSorted.size(); ++i)
    {
        QString qstr = QString::fromStdString(DrugSorted[i]->name);
        QString qval = QString::number(DrugSorted[i]->error);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(qstr));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(qval));
    }
    if (!isColorSet1) colorMid   = Color(-1);
    if (!isColorSet2) colorStop  = Color(-1);

}

void MainWindow::resetTable()
{
    ui->tableWidget->setRowCount(0);

    std::vector<Drug *>::iterator it = DrugSorted.begin();
    for (; it != DrugSorted.end(); ++it)
    {
        (*it) -> error = 0;
    }

    DrugSorted.erase(DrugSorted.begin(), DrugSorted.end());
}

void MainWindow::editDataBase()
{
    /*qDebug()<<QString::fromStdString(DrugList[activeDrug].name);
    qDebug()<<activeTest;
    qDebug()<<QString::fromStdString(colorMid.toHex());
    qDebug()<<QString::fromStdString(colorStop.toHex());*/

    std::ifstream infile("drugs.txt");
    std::string line;
    std::string data;

    while (std::getline(infile, line))
    {
        Drug tmp;
        tmp.loadFromLine(line);

        if (tmp.name.compare(DrugList[activeDrug].name) == 0)
        {
            //qDebug()<<QString::fromStdString(tmp.toLine());
            tmp.test[activeTest*2].copyFrom(colorMid);
            tmp.test[activeTest*2+1].copyFrom(colorStop);
            //qDebug()<<QString::fromStdString(tmp.toLine());
            data = data + tmp.toLine() + "\n";
        }
        else data = data + line + "\n";
    }

    std::ofstream fileout("drugs.txt");

    fileout << data;

    DrugList.clear();

    std::ifstream infileNew("drugs.txt");

    while (std::getline(infileNew, line))
    {
        Drug tmp;
        tmp.loadFromLine(line);
        DrugList.push_back(tmp);
    }

    repaint();
}

bool DrugCompare(Drug *x, Drug *y) { return x->error < y->error; }

void CheckErrors(const Color &sample1, const Color &sample2, int testFlag)
{
    std::vector<Drug *>::iterator it = DrugSorted.begin();
    for (; it != DrugSorted.end(); ++it)
    {
        Color ref1 = (*it) -> test[testFlag * 2];
        Color ref2 = (*it) -> test[testFlag * 2 + 1];

        qDebug()<<QString::fromStdString((*it) -> name);

        (*it) -> error += CopmareTests(ref1, ref2, sample1, sample2, testFlag);
    }
    std::sort(DrugSorted.begin(), DrugSorted.end(), DrugCompare);

    while (DrugSorted[DrugSorted.size() - 1] -> error > ERROR_THRESHOLD)
        DrugSorted.pop_back();

}

unsigned int lev_dam_dist(std::string s1,  std::string s2)
{
  size_t size1 = s1.size();
  size_t size2 = s2.size();
  size_t d[size1 + 1][size2 + 1];
  for (unsigned int i = 0; i <= size1; i ++)
    d[i][0] = i;
  for (unsigned int i = 0; i <= size2; i ++)
    d[0][i] = i;

  int cost = 0;
  for (unsigned int i = 1; i <= size1; i ++)
    for (unsigned int j = 1; j <= size2; j ++)
    {
      cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1 ;
      if ( (i > 1) && (j > 1) && (s1[i] == s2[j - 1]) && (s1[i - 1] == s2[j]))
      {
        size_t a = std::min(d[i - 1][j], d[i][j - 1] + 1);
        size_t b = std::min(d[i][j] + cost, d[i - 2][j - 2]);
        d[i][j] = std::min(a, b);
      }
      else
      {
        d[i][j] = std::min(std::min(d[i][j -1] + 1, d[i - 1][j] + 1), d[i - 1][j - 1] + cost);
      }
    }
  return d[size1][size2];
}

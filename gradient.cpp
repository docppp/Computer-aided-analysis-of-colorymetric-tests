#include "gradient.h"
#include "drug.h"
#include <string>

gradient::gradient(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    setBg(1);
}

void gradient::setBg(int test)
{
    QPalette p = palette();

    p.setColor(QPalette::Background, fromHex(testColor(test)));

    this->setPalette(p);
}

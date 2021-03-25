#ifndef GRADIENT_H
#define GRADIENT_H

#include <QWidget>

class gradient : public QWidget
{
    Q_OBJECT
public:
    explicit gradient(QWidget *parent = 0);

signals:

public slots:
    void setBg(int test);

};

#endif // GRADIENT_H

#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <QColor>

class Color
{
public:
    std::string name;

    int R;
    int G;
    int B;

    Color(int r = 0, int g = 0, int b = 0);

    void inBetween(Color c1, Color c2);

    void copyFrom(Color c);

    int loadFromHex(std::string h);

    std::string toHex() const;
};

std::ostream &operator<<(std::ostream &os, Color &c);

double ColorDistance(const Color &x, const Color &y);

QColor fromHex(std::string hex);

#include "color.h"
#include <cmath>
#include <QDebug>
#include <QString>

#define LOG std::cout<<
#define TEST

Color::Color(int r, int g, int b)
{
    R = r;
    G = g;
    B = b;
}

void Color::inBetween(Color c1, Color c2)
{
    R = (c1.R + c2.R) / 2;
    G = (c1.G + c2.G) / 2;
    B = (c1.B + c2.B) / 2;
}

void Color::copyFrom(Color c)
{
    R = c.R;
    G = c.G;
    B = c.B;
}

int Color::loadFromHex(std::string h)
{
    if (h.empty())
    {
        R = -1, G = -1, B = -1;

        /*#ifdef TEST
        LOG "COLOR: WARNING! Load empty hex. Color value = -1.\n";
        #endif // TEST*/
        return 0;
    }

    if (h.size() != 6 || h.find_first_not_of("0123456789ABCDEF") != std::string::npos)
    {
        #ifdef TEST
        LOG "COLOR: ERROR! Cannot load color from string. '" <<
            h << "' is not a valid hex.\n";
        #endif // TEST
        return -1;
    }

    std::string tmp;

    tmp = h.substr(0, 2);
    R = (int)strtol(tmp.c_str(), NULL, 16);

    tmp = h.substr(2, 2);
    G = (int)strtol(tmp.c_str(), NULL, 16);

    tmp = h.substr(4, 2);
    B = (int)strtol(tmp.c_str(), NULL, 16);

    return 1;
}

static std::string DecimalToHexadecimal(int dec)
{
    if (dec < 1) return "00";

    int hex = dec;
    std::string hexStr = "";

    while (dec > 0)
    {
        hex = dec % 16;

        if (hex < 10)
            hexStr = hexStr.insert(0, std::string(1, (hex + 48)));
        else
            hexStr = hexStr.insert(0, std::string(1, (hex + 55)));

        dec /= 16;
    }

    if (hexStr.size() == 1)
    {
        hexStr = hexStr.insert(0, std::string("0"));
    }

    return hexStr;
}

std::string Color::toHex() const
{
    /*std::stringstream ss;
    std::string tmp;

    ss << std::hex << R;
    tmp += ss.str();

    ss << std::hex << G;
    tmp += ss.str();

    ss << std::hex << B;
    tmp += ss.str();

    return tmp;*/
    std::string rs = DecimalToHexadecimal(R);
     std::string gs = DecimalToHexadecimal(G);
      std::string bs = DecimalToHexadecimal(B);

        return rs + gs + bs;
}

std::ostream &operator<<(std::ostream &os, Color &c)
{
    return os << c.name << " " << c.R << " " << c.G << " " << c.B;
}

static double * rgb2CIELAB(Color c)
{
    static double val[3];
    double r = ((double) c.R) / 255.0;
    double g = ((double) c.G) / 255.0;
    double b = ((double) c.B) / 255.0;
    qDebug()<<r<<"-"<<g<<"-"<<b;
    double x, y, z;

    r = (r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
    g = (g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
    b = (b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92;
    qDebug()<<r<<"-"<<g<<"-"<<b;
    x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
    y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
    z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;
    qDebug()<<x<<"-"<<y<<"-"<<z;
    x = (x > 0.008856) ? pow(x, 0.33333333) : (7.787 * x) + 16/116;
    y = (y > 0.008856) ? pow(y, 0.33333333) : (7.787 * y) + 16/116;
    z = (z > 0.008856) ? pow(z, 0.33333333) : (7.787 * z) + 16/116;
    qDebug()<<x<<"-"<<y<<"-"<<z;
    val[0] = 116 * y - 16;
    val[1] = 500 * (x - y);
    val[2] = 200 * (y - z);
    qDebug()<<val[0]<<"-"<<val[1]<<"-"<<val[2];
    return val;
}

double ColorDistance(const Color &x, const Color &y)
{
    double labA[3], labB[3];
    double *p, *q;

    p = rgb2CIELAB(x);
    for (int i=0; i<3; ++i) labA[i]=*(p+i);
    qDebug()<<"RGB "<<x.R<<" "<<x.G<<" "<<x.B<<" = LAB "<<labA[0]<<" "<<labA[1]<<" "<<labA[2];
    qDebug()<<"-----------";
    q = rgb2CIELAB(y);
    for (int i=0; i<3; ++i) labB[i]=*(q+i);
    qDebug()<<"RGB "<<x.R<<" "<<x.G<<" "<<x.B<<" = LAB "<<labB[0]<<" "<<labB[1]<<" "<<labB[2];

    /*double deltaL = labA[0] - labB[0];
    double deltaA = labA[1] - labB[1];
    double deltaB = labA[2] - labB[2];
    double c1 = sqrt(labA[1] * labA[1] + labA[2] * labA[2]);
    double c2 = sqrt(labB[1] * labB[1] + labB[2] * labB[2]);
    double deltaC = c1 - c2;
    double deltaH = deltaA * deltaA + deltaB * deltaB - deltaC * deltaC;
    deltaH = deltaH < 0 ? 0 : sqrt(deltaH);
    double sc = 1.0 + 0.045 * c1;
    double sh = 1.0 + 0.015 * c1;
    double deltaLKlsl = deltaL / (1.0);
    double deltaCkcsc = deltaC / (sc);
    double deltaHkhsh = deltaH / (sh);
    double i = deltaLKlsl * deltaLKlsl + deltaCkcsc * deltaCkcsc + deltaHkhsh * deltaHkhsh;*/
    double i = pow(labA[0] - labB[0], 2) + pow(labA[1] - labB[1], 2) + pow(labA[2] - labB[2], 2);
    qDebug()<<"DIFF: "<<i;
    return sqrt(i);
}

static int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

QColor fromHex(std::string hex)
{
    int r = hex2int(hex[0])*16+hex2int(hex[1]);
    int g = hex2int(hex[2])*16+hex2int(hex[3]);
    int b = hex2int(hex[4])*16+hex2int(hex[5]);

    QColor c(r, g, b);

    return c;
}


#include "drug.h"
#include <QDebug>
#include <QString>

#define LOG std::cout<<
#define TEST

int Drug::loadFromLine(std::string line)
{
    int n = 0;

    for (int i = 0; i < line.size(); i++) if (line[i] == ';') n++;

    if (n != NUMBER_OF_TESTS * 2)
    {
        #ifdef TEST
        LOG "DRUG: ERROR! Line " << line << " is not valid. Wrong number" <<
            "of delimiters.\n";
        #endif // TEST
        return -1;
    }

    int pos = -1;
    int posPrev = -1;
    int i = 0;

    std::string data[NUMBER_OF_TESTS * 2 + 1];
    std::string delimiter = ";";

    do
    {
        posPrev = pos;
        pos = line.find(delimiter, pos + 1);

        data[i++] = line.substr(posPrev + 1, pos - posPrev - 1);

    } while (pos != std::string::npos);

    if (data[0].empty())
    {
        #ifdef TEST
        LOG "DRUG: ERROR! Line '" << line << "' is not valid. " <<
            "Drug must have a name.\n";
        #endif // TEST
        return -1;
    }

    name = data[0];

    for (i = 1; i < NUMBER_OF_TESTS * 2 + 1; i++)
    {
        test[i - 1].name = data[i];

        if (test[i - 1].loadFromHex(data[i]) == -1)
        {
            #ifdef TEST
            LOG "DRUG: ERROR! Cannot load line for drug '" << data[0] << "'.\n";
            #endif // TEST
            return -1;
        }
    }

    error = 0;

    return 1;
}

std::string Drug::toLine()
{
    std::string result;

    result = result + name;

    for (int i = 0; i < NUMBER_OF_TESTS * 2; ++i)
    {
        if (test[i].R == -1) result = result + ";";
        else result = result + ";" + test[i].toHex();
    }

    return result;
}

std::string testColor(int test)
{
    switch (test)
    {
    case MARQUIS:   return MARQUIS_COLOR;
    case MECKE:     return MECKE_COLOR;
    case MANDELIN:  return MANDELIN_COLOR;
    case LIEBERMANN:return LIEBERMANN_COLOR;
    case FROEHDE:   return FROEHDE_COLOR;
    case ROBADOPE:  return ROBADOPE_COLOR;
    case SIMONS:    return SIMONS_COLOR;
    case FOLIN:     return FOLIN_COLOR;
    case EHRLICH:   return EHRLICH_COLOR;
    case HOFMANN:   return HOFMANN_COLOR;
    case SCOTT:     return SCOTT_COLOR;
    case GALUS:     return GALUS_COLOR;
    case ZIMMERMAN: return ZIMMERMAN_COLOR;
    }
}

double CopmareTests(Color ref1, Color ref2, const Color &sample1, const Color &sample2, int testFlag)
{
    if (testFlag < 0 || testFlag >= NUMBER_OF_TESTS * 2)
    {
        #ifdef TEST
        LOG "Drug: ERROR! Cannot compare tests with flag '" << testFlag << "'.\n";
        #endif // TEST
        return -1;
    }

    if (ref1.R == -1 && ref2.R == -1)
    {
        qDebug()<<"Returning error bias";
        return ERROR_BIAS;
    }

    Color colorStart;
    double midError;
    double endError;

    colorStart.loadFromHex(testColor(testFlag));

    if (ref1.R == -1 && ref1.G == -1 && ref1.B == -1)
        ref1.inBetween(colorStart, ref2);

    std::string s1 = ref1.toHex();
    std::string s2 = ref2.toHex();
    const std::string s3 = sample1.toHex();
    const std::string s4 = sample2.toHex();

    qDebug()<<testFlag<<" R1"<<QString::fromStdString(s1)
           <<" R2"<<QString::fromStdString(s2)
             <<" S1"<<QString::fromStdString(s3)
               <<" S2"<<QString::fromStdString(s4);

    midError = ColorDistance(ref1, sample1);
    endError = ColorDistance(ref2, sample2);

    return midError + endError;
}

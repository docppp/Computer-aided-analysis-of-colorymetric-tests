#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include "color.h"

#define NUMBER_OF_TESTS 13

#define MARQUIS1		1
#define MARQUIS2		2
#define MARQUIS_COLOR	"FFFFFF"
#define MECKE1			3
#define MECKE2			4
#define MECKE_COLOR		"FFFFFF"
#define MANDELIN1		5
#define MANDELIN2		6
#define MANDELIN_COLOR	"FFFF00"
#define LIEBERMANN1		7
#define LIEBERMANN2		8
#define LIEBERMANN_COLOR "FFFFFF"
#define FROEHDE1		9
#define FROEHDE2		10
#define FROEHDE_COLOR	"FFFFFF"
#define ROBADOPE1		11
#define ROBADOPE2		12
#define ROBADOPE_COLOR	"FFFFFF"
#define SIMONS1			13
#define SIMONS2			14
#define SIMONS_COLOR	"FDE9D9"
#define FOLIN1			15
#define FOLIN2			16
#define FOLIN_COLOR		"FFFFFF"
#define EHRLICH1		17
#define EHRLICH2		18
#define EHRLICH_COLOR	"FFFFFF"
#define HOFMANN1		19
#define HOFMANN2		20
#define HOFMANN_COLOR	"FFFFFF"
#define SCOTT1			21
#define SCOTT2			22
#define SCOTT_COLOR		"FF6FCF"
#define GALUS1			23
#define GALUS2			24
#define GALUS_COLOR		"8E563A"
#define ZIMMERMAN1		25
#define ZIMMERMAN2		26
#define ZIMMERMAN_COLOR	"FFFFFF"

#define MARQUIS		0
#define MECKE		1
#define MANDELIN	2
#define LIEBERMANN	3
#define FROEHDE		4
#define ROBADOPE	5
#define SIMONS		6
#define FOLIN		7
#define EHRLICH		8
#define HOFMANN		9
#define SCOTT		10
#define GALUS		11
#define ZIMMERMAN	12

#define ERROR_THRESHOLD 20
#define ERROR_BIAS 1.5
#define ERROR_MULTI 1

class Drug
{
public:
    std::string name;

    Color test[NUMBER_OF_TESTS * 2];

    double error;

    int loadFromLine(std::string line);

    bool operator< (Drug const &q) const { return error < q.error; }

    std::string toLine();
};

std::string testColor(int test);

double CopmareTests(Color ref1, Color ref2, const Color &sample1, const Color &sample2, int testFlag);



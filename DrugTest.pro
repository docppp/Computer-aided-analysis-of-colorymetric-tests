#-------------------------------------------------
#
# Project created by QtCreator 2018-08-11T17:54:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrugTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    gradient.cpp \
    color.cpp \
    drug.cpp \

HEADERS  += mainwindow.h \
    gradient.h \
    color.h \
    drug.h

FORMS    += mainwindow.ui

DISTFILES += \
    libgcc_s_dw2-1.dll \
    libstdc++-6.dll \
    libwinpthread-1.dll \
    Qt5Core.dll \
    Qt5Gui.dll \
    Qt5Widgets.dll

CONFIG += static

#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T17:05:52
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watchme
TEMPLATE = app


SOURCES += main.cpp\
    systeminfo_x11.cpp \
    crawler.cpp

HEADERS  += \
    SystemInfo.h \
    crawler.h

FORMS    +=

RESOURCES += \
    icons.qrc

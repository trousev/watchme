#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T17:05:52
#
#-------------------------------------------------

QT       += core gui sql
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watchme
TEMPLATE = app


SOURCES += main.cpp\
    systeminfo_x11.cpp \
    crawler.cpp \
    transformer.cpp \
    logic.cpp \
    viewreport.cpp \
    nightcharts.cpp

HEADERS  += \
    SystemInfo.h \
    crawler.h \
    transformer.h \
    logic.h \
    viewreport.h \
    nightcharts.h

FORMS    += \
    transformer.ui \
    viewreport.ui

RESOURCES += \
    icons.qrc

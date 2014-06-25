#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T17:05:52
#
#-------------------------------------------------

QT       += core gui sql
# QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watchme
TEMPLATE = app


SOURCES += main.cpp\
    systeminfo_x11.cpp \
    crawler.cpp \
    logic.cpp \
    uconfigurator.cpp \
    uaddfilter.cpp

HEADERS  += \
    SystemInfo.h \
    crawler.h \
    logic.h \
    uconfigurator.h \
    uaddfilter.h

FORMS    += \
    uconfigurator.ui \
    uaddfilter.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    sheet.sql

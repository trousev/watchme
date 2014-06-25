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


SOURCES += src/main.cpp\
    src/systeminfo_x11.cpp \
    src/crawler.cpp \
    src/logic.cpp \
    src/uconfigurator.cpp \
    src/uaddfilter.cpp

HEADERS  += \
    src/SystemInfo.h \
    src/crawler.h \
    src/logic.h \
    src/uconfigurator.h \
    src/uaddfilter.h

FORMS    += \
    src/uconfigurator.ui \
    src/uaddfilter.ui

RESOURCES += \
    res/res.qrc

OTHER_FILES += \
    res/sheet.sql


OBJECTS_DIR=$$PWD/tmp
MOC_DIR=$$PWD/tmp
RCC_DIR=$$PWD/tmp
UI_DIR=$$PWD/tmp
DESTDIR=$$PWD/bin


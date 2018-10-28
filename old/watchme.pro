#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T17:05:52
#
#-------------------------------------------------

QT       += core gui sql webkit
# QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

TARGET = watchme
TEMPLATE = app

unix: !macx: SOURCES += src/systeminfo_x11.cpp
macx: OBJECTIVE_SOURCES +=  src/systeminfo_mac.mm
macx: LIBS += -framework Cocoa
macx: QMAKE_INFO_PLIST = res/Info.plist
macx: ICON = res/eye.icns

SOURCES += src/main.cpp\
    src/crawler.cpp \
    src/logic.cpp \
    src/uconfigurator.cpp \
    src/uaddfilter.cpp \
    detectidle.cpp

HEADERS  += \
    src/SystemInfo.h \
    src/crawler.h \
    src/logic.h \
    src/uconfigurator.h \
    src/uaddfilter.h \
    detectidle.h

FORMS    += \
    src/uconfigurator.ui \
    src/uaddfilter.ui

RESOURCES += \
    res/res.qrc \
    chartjs.qrc

OTHER_FILES += \
    res/sheet.sql \
    res/index.html \
    res/pattern.sql \
    res/Info.plist


OBJECTS_DIR=$$PWD/tmp
MOC_DIR=$$PWD/tmp
RCC_DIR=$$PWD/tmp
UI_DIR=$$PWD/tmp
DESTDIR=$$PWD/bin


#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T17:05:52
#
#-------------------------------------------------

QT       += core gui sql
# QT += webkit
# QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

TARGET = watchme
TEMPLATE = app

unix: !macx: SOURCES += src/systeminfo_x11.cpp
macx: OBJECTIVE_SOURCES +=  src/systeminfo_mac.mm
macx: LIBS += -framework Cocoa
macx: QMAKE_INFO_PLIST =
macx: ICON = res/eye.icns

SOURCES += \
    src/SystemInfo.cpp \
    src/uMain.cpp \
    src/crawler.cpp

HEADERS  += \
    src/SystemInfo.h \
    src/crawler.h

FORMS    +=

RESOURCES += \
    res/res.qrc \
    chartjs.qrc

OTHER_FILES += \
    DEPENDS.md


OBJECTS_DIR=$$PWD/tmp
MOC_DIR=$$PWD/tmp
RCC_DIR=$$PWD/tmp
UI_DIR=$$PWD/tmp
DESTDIR=$$PWD/bin

INCLUDEPATH+=$$PWD

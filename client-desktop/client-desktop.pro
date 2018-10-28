#-------------------------------------------------
#
# Project created by QtCreator 2018-10-28T17:54:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client-desktop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        uwatchme.cpp \
    utracker.cpp \
    systeminfo.cpp

HEADERS += \
        uwatchme.h \
        systeminfo.h \
    utracker.h

FORMS += \
        uwatchme.ui



unix: !macx: SOURCES += systeminfo_x11.cpp
unix: !macx: LIBS += -lX11

macx: OBJECTIVE_SOURCES +=  systeminfo_mac.mm
macx: LIBS += -framework Cocoa
macx: QMAKE_INFO_PLIST = Info.plist
macx: ICON = eye.icns

RESOURCES += \
    res.qrc


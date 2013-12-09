#include <QApplication>
#include "SystemInfo.h"
#include <QIcon>
#include <QMenu>
#include <QDebug>
#include "crawler.h"
#include <QSystemTrayIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Crawler l;

    QMenu m;

    QAction * actionExit = m.addAction("Exit watcher");
    QObject::connect(actionExit, SIGNAL(triggered()), &a, SLOT(quit()));

    QSystemTrayIcon i(QIcon(":/icons/eye.png"));
    i.setContextMenu(&m);
    i.show();


    return a.exec();
}

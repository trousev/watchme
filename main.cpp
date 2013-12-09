#include <QApplication>
#include "SystemInfo.h"
#include <QIcon>
#include <QMenu>
#include <QDebug>
#include "crawler.h"
#include "logic.h"
#include <QSystemTrayIcon>
#include <QSharedMemory>
#include "viewreport.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);


    Crawler crawler;
    Logic logic;
    QMenu m;

    ViewReport report(NULL,&logic, &crawler);

    QObject::connect(m.addAction("Show Report"), SIGNAL(triggered()), &report, SLOT(update()));
    QObject::connect(m.addAction("Exit watcher"), SIGNAL(triggered()), &a, SLOT(quit()));

    QSystemTrayIcon i(QIcon(":/icons/eye.png"));
    i.setContextMenu(&m);
    i.show();

    return a.exec();
}

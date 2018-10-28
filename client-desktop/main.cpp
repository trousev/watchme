#include "uwatchme.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include "utracker.h"
#include <QTimer>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("CPST");
    QCoreApplication::setOrganizationDomain("trousev.pro");
    QCoreApplication::setApplicationName("WatchMe");


    QSystemTrayIcon tray(QIcon(":/eye.png"));
    QMenu * menu = new QMenu();
    UTracker tracker("HERE_SHOULD_BE_API_KEY");
    UWatchMe w(&tracker);
    QTimer * timer = new QTimer;

    QAction * exitAction = menu->addAction("Exit");
    QObject::connect(exitAction, SIGNAL(triggered(bool)), &a, SLOT(quit()));
    tray.setContextMenu(menu);

    QObject::connect(&tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), &w, SLOT(show()));

    timer->setInterval(1000);
    QObject::connect(timer, SIGNAL(timeout()), &tracker, SLOT(track()));
    timer->start();

    QObject::connect(&w,SIGNAL(somethingChanged(QString,QString)), &tray, SLOT(showMessage(QString,QString)));

    QSettings s;
    if( !s.value("loggedIn", false).toBool() )
    {
        w.show();
    }
    tray.show();

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}

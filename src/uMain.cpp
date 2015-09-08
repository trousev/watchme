#include "SystemInfo.h"
#include <QDebug>
#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "crawler.h"
int main(int q, char * argv[]) {
    QCoreApplication::setOrganizationName("CpyPstStd");
    QCoreApplication::setOrganizationDomain("trousev.pro");
    QCoreApplication::setApplicationName("WatchMe");
    QApplication app(q, argv);



    QSystemSemaphore sema("WATCHME.9aa3f102-0147-4b02-a540-3a5807ac0be7", 1);
    bool isRunning;
    sema.acquire();

    {
        QSharedMemory shmem("WATCHME.3b772c5d-eb27-4f2c-bbed-02b32231c862");
        shmem.attach();
    }

    QSharedMemory shmem("WATCHME.3b772c5d-eb27-4f2c-bbed-02b32231c862");
    if (shmem.attach())
    {
        isRunning = true;
        qDebug() << "Application is already running";
        exit(0);
    }
    else
    {
        shmem.create(1);
        isRunning = false;
    }
    sema.release();


    Crawler * c = new Crawler();
    return app.exec();
}
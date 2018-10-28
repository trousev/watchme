#ifndef UTRACKER_H
#define UTRACKER_H

#include <QObject>
#include "systeminfo.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QMutex>

struct Report {
    WindowInfo subject;
    QString device_key;
    QDateTime moment;
};

struct ApiCallError {
    QString message;
    int code;
    ApiCallError(QString message, int code=-1): message(message), code(code) {}
};

class UTracker : public QObject
{
    Q_OBJECT

    QMutex _reportsMutex;
    QString _api_key;
    QDateTime _lastReport;
    WindowInfo _lastInfo;
    QList<Report> _reports;
    QNetworkAccessManager * net;
public:
    explicit UTracker(QString api_key, QObject *parent = nullptr);
    QVariant apicall(QString resource, QVariant body = QVariant());
    void updateApiKey(QString api_key);

signals:
    void somethingChanged(QString title, QString message);

public slots:
    void track();

private:
    void addReport(WindowInfo wi);
    void save();
    void load();
    void sync();

};

#endif // UTRACKER_H

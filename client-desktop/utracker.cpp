#include "utracker.h"
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QVariantMap>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QEventLoop>
#include <Qt>

UTracker::UTracker(QString api_key, QObject *parent) : QObject(parent), _api_key(api_key), _lastReport(QDateTime::currentDateTime()), net(new QNetworkAccessManager) {
    load();
    sync();
}

void UTracker::track() {
    QSettings settings;
    int maximumSeconds = settings.value("MandatorySendInterval", 60).toInt();
    if(maximumSeconds<=0) maximumSeconds = 60;

    auto wi = getWindowInfo();
    int secondsPassed = _lastReport.secsTo(QDateTime::currentDateTime());
    if(wi != _lastInfo || secondsPassed > maximumSeconds) {
        addReport(wi);
    }
}
void UTracker::updateApiKey(QString api_key) {
    this->_api_key = api_key;
}

void UTracker::addReport(WindowInfo wi) {
    QDateTime moment = QDateTime::currentDateTime();
    qDebug() << "REPORTED: " << moment << wi.windowClass << wi.windowName << wi.windowTitle;
    _lastInfo = wi;
    _lastReport = moment;

    Report r;
    r.device_key = _api_key;
    r.moment = moment;
    r.subject = wi;
    _reportsMutex.lock();
    _reports.push_back(r);
    _reportsMutex.unlock();

    save();
    sync();

}



void UTracker::load() {
    QDir outputDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QFile outputFile(outputDir.absoluteFilePath("unsynced-data.json"));
    if(!outputFile.open(QFile::ReadOnly)) {
        qDebug() << "Unable to open unsynced-data file. ";
        return;
    }

    QJsonArray arr = QJsonDocument::fromJson(outputFile.readAll()).array();
    for(QJsonValue v: arr) {
        QVariantMap rep = v.toVariant().value<QVariantMap>();
        Report r;
        r.moment = rep["moment"].toDateTime();
        r.device_key = rep["device"].toString();
        r.subject.windowClass = rep["application"].toString();
        r.subject.windowName = r.subject.windowTitle = rep["context"].toString();
        _reportsMutex.lock();
        _reports.push_back(r);
        _reportsMutex.unlock();
    }

}

void UTracker::sync() {
    _reportsMutex.lock();
    bool needToSave = false;
    QVariantList reports;
    for(Report r: _reports) {
        QVariantMap map;
        map["device"] = r.device_key;
        map["moment"] = r.moment.toString(Qt::DateFormat::RFC2822Date);
        map["application"] = r.subject.windowClass;
        map["context"] = r.subject.windowTitle;
        reports.append(map);
    }
    try {
        apicall("track", reports);
        _reports.clear();
        needToSave = true;
    } catch (ApiCallError e) {
        if(e.code == 666) {
            // This package IS DOOMED. Just skip it =(
            emit somethingChanged("Sync problem", "Unable to sync with upstream server. Some data is lost.");
            _reports.clear();
            needToSave = true;
        }
        qDebug() << "Sync problems " << e.message;
    }
    if(needToSave) save();
    _reportsMutex.unlock();
}

void UTracker::save() {
    QDir outputDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    outputDir.mkpath("kuku");
    QFile outputFile(outputDir.absoluteFilePath("unsynced-data.json"));
    outputFile.open(QFile::WriteOnly);

    QJsonArray reports;
    for(Report r: _reports) {
        QVariantMap map;
        map["device"] = r.device_key;
        map["moment"] = r.moment;
        map["application"] = r.subject.windowClass;
        map["context"] = r.subject.windowTitle;
        reports.append(QJsonValue::fromVariant(map));
    }
    outputFile.write(QJsonDocument(reports).toJson());
    outputFile.close();
}

QVariant UTracker::apicall(QString resource, QVariant body) {
    QSettings settings;
    QUrl url = QUrl(QString("%1/%2").arg(settings.value("endpoint", "http://example.com").toString()).arg(resource));
    QNetworkRequest req( url );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "Accessing URL " << url;
    QNetworkReply * reply;
    if(body.isNull()) {
        reply = net->get(req);
    } else {
        reply = net->post(req, QJsonDocument::fromVariant(body).toJson());
    }
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error()) throw ApiCallError(reply->errorString(), reply->error());

    QVariantMap answer = QJsonDocument::fromJson(reply->readAll()).toVariant().value<QVariantMap>();
    if(answer["status"].toString() == "") {
        throw ApiCallError("Unable to parse JSON");
    }
    if(answer["status"].toString() == "failure") {
        throw ApiCallError("API unrecoverable failure", 666);
    }
    if(answer["status"].toString() != "success") {
        throw ApiCallError( answer["message"].toString());
    }
    return answer;
}

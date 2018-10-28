#include "uwatchme.h"
#include "ui_uwatchme.h"
#include "systeminfo.h"
#include <QVariantMap>
#include <QSettings>
#include <QMessageBox>

UWatchMe::UWatchMe(UTracker *tracker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UWatchMe),
    _tracker(tracker)
{
    ui->setupUi(this);
    reloadUi();
}
void UWatchMe::reloadUi() {
    QSettings s;
    ui->_endpoint->setText(s.value("endpoint", "http://localhost:8000").toString());
    ui->_device_id->setText(s.value("device", "").toString());
    ui->stackedWidget->setCurrentIndex( s.value("loggedIn", false).toBool() ? 1 : 0 );
    _tracker->updateApiKey(ui->_device_id->text());
}

UWatchMe::~UWatchMe()
{
    delete ui;
}

void UWatchMe::on__login_clicked()
{
    QSettings s;
    s.setValue("endpoint", ui->_endpoint->text());
    s.setValue("device", ui->_device_id->text());

    QVariantMap body;
    body["device"] = ui->_device_id->text();
    try {
        _tracker->apicall("devicelogin", body);
        s.setValue("loggedIn", true);
        hide();
        emit somethingChanged("Logged in", "WatchMe is logging your activity.");
    } catch (ApiCallError e) {
        QMessageBox::critical(this, "Unable to login", e.message);
    }
    reloadUi();
}

void UWatchMe::on__logout_clicked()
{
    QSettings s;
    s.setValue("loggedIn", false);
    reloadUi();
}

void UWatchMe::on_pushButton_clicked()
{
    QApplication::exit();
}

void UWatchMe::on_pushButton_2_clicked()
{
    QApplication::exit();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <bluetooth/selectbluetoothform.h>
#include <QSettings>
#include <QClipboard>
#include <QJniObject>
#include <QMessageBox>
#include <Settings.h>

//#include <yandexbanner.h>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void saveSetting(const QString &key, const QString &value);

private:
    QNetworkAccessManager* networkManager;

private slots:
    void onApiRequestFinished(QNetworkReply* reply);
    void on_pushButtonSend_clicked();

    void on_pushButton_bluetooth_clicked();

    void on_pushButton_clear_clicked();
    QString loadSetting(const QString &key, const QString &defaultValue = QString());

    void on_pushButton_clicked();

public slots:
    void SendMessage(QString str);

    void slot_keyChanged(QString key);

private:
    Ui::MainWindow *ui;
    SelectBluetoothForm *bluetooth;
    Settings *settings;
    QString yourChatGPT_ID;
};
#endif // MAINWINDOW_H

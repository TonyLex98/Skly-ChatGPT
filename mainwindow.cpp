#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtCore/private/qandroidextras_p.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bluetooth = new SelectBluetoothForm();

    settings = new Settings();

    connect(bluetooth,&SelectBluetoothForm::firstWindow, this, &MainWindow::show);

    networkManager = new QNetworkAccessManager(this);

    // Подключаем слот обработки ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onApiRequestFinished);

    // Подключаем слот обработки нажатия кнопки
    //connect(ui->pushButtonSend, &QPushButton::clicked, this, &MainWindow::on_pushButtonSend_clicked);

    connect(bluetooth, &SelectBluetoothForm::signalInputData, this, &MainWindow::SendMessage);
    connect(settings, &Settings::keyChanged, this, &MainWindow::slot_keyChanged);


    //ui->YandexAdd->initializeBanner("R-M-2313590-1");

    //YandexBanner *yandexbanner = new YandexBanner();
    //ui->verticalLayout->addWidget(yandexbanner);

    yourChatGPT_ID = loadSetting("yourChatGPT_ID","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");

    ui->label_help_patreon->setText("<a href=\"https://patreon.com/Skly\">patreon.com/Skly</a>");
    ui->label_help_patreon->setTextFormat(Qt::RichText);
    ui->label_help_patreon->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByMouse);
    connect(ui->label_help_patreon, &QLabel::linkActivated, [=](const QString &link) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(link);
        QMessageBox::warning(this,tr("The link is copied"),tr("please subscribe to patreon.com/Skly"));
    });

    ui->label_help_boosty->setText("<a href=\"https://boosty.to/skly_youtube\">boosty.to/skly_youtube</a>");
    ui->label_help_boosty->setTextFormat(Qt::RichText);
    ui->label_help_boosty->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByMouse);
    connect(ui->label_help_boosty, &QLabel::linkActivated, [=](const QString &link) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(link);
        QMessageBox::warning(this,tr("The link is copied"),tr("please subscribe to boosty.to/skly_youtube"));
    });

    ui->label_youtube->setText("<a href=\"https://www.youtube.com/@skly6453\">Skly Youtube</a>");
    ui->label_youtube->setTextFormat(Qt::RichText);
    ui->label_youtube->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByMouse);
    connect(ui->label_youtube, &QLabel::linkActivated, [=](const QString &link) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(link);
        QMessageBox::warning(this,tr("The link is copied"),tr("please subscribe to https://www.youtube.com/@skly6453"));
    });

    //https://www.youtube.com/@skly6453

    ui->textEditRequest->setPlaceholderText(tr("Write here"));
    ui->textBrowserResponse->setPlaceholderText(tr("The answer will be here"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onApiRequestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        // Читаем ответ
        QByteArray responseData = reply->readAll();

        // Разбираем JSON
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        // Получаем ответ от GPT
        QString gptResponse = jsonObj["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();


        //qDebug()<<"gptResponse = "<<jsonObj;
        //QJsonDocument doc(jsonObj);
        //QString strJson(doc.toJson(QJsonDocument::Compact));
        // Выводим результат в textEditResponse
        ui->textBrowserResponse->append("Chat GPT 3.5:\n" + gptResponse + '\n');
        //qDebug()<<"gptResponse.toLocal8Bit() = "<<gptResponse.toLocal8Bit();

        //BluetoothModule::getInstance()->socket->write(gptResponse.toLocal8Bit());
        BluetoothModule::getInstance()->sendData(QByteArray(gptResponse.toLocal8Bit()));
    }
    else
    {
        //qDebug() << "Error:" << reply->errorString();
        ui->textBrowserResponse->append( reply->errorString()+'\n');
        //qDebug()<<"reply->errorString().toLocal8Bit() = "<<reply->errorString().toLocal8Bit();
        BluetoothModule::getInstance()->sendData(QByteArray(reply->errorString().toLocal8Bit()));
    }

    reply->deleteLater();
}

void MainWindow::on_pushButtonSend_clicked()
{
    //qDebug()<<"MainWindow::on_pushButtonSend_clicked()";
    SendMessage(ui->textEditRequest->toPlainText());
/*
    // Создаем запрос
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.openai.com/v1/chat/completions"));

    // Устанавливаем заголовки
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer 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"));

    // Создаем JSON-данные
    QJsonObject message;
    message["role"] = "user";
    message["content"] = ui->lineEditRequest->text();

    QJsonArray messages;
    messages.append(message);

    QJsonObject data;
    data["model"] = "gpt-3.5-turbo";
    data["messages"] = messages;

    QJsonDocument jsonDoc(data);
    QByteArray jsonData = jsonDoc.toJson();

    // Отправляем POST-запрос
    networkManager->post(request, jsonData);

    //ui->lineEditRequest->setText(str);
    */

}

void MainWindow::on_pushButton_bluetooth_clicked()
{
    bluetooth->show();
    this->hide();
}

void MainWindow::SendMessage(QString str)
{
    // Создаем запрос
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.openai.com/v1/chat/completions"));

    // Устанавливаем заголовки
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer "+ yourChatGPT_ID.toLocal8Bit()));

    // Создаем JSON-данные
    QJsonObject message;
    message["role"] = "user";
    message["content"] = str;

    QJsonArray messages;
    messages.append(message);

    QJsonObject data;
    data["model"] = "gpt-3.5-turbo";
    data["messages"] = messages;

    QJsonDocument jsonDoc(data);
    QByteArray jsonData = jsonDoc.toJson();

    // Отправляем POST-запрос
    networkManager->post(request, jsonData);

    ui->textEditRequest->clear();
    ui->textBrowserResponse->append(tr("You:")+'\n' + str + '\n');
}

void MainWindow::slot_keyChanged(QString key)
{
    saveSetting("yourChatGPT_ID",key);
    yourChatGPT_ID = loadSetting("yourChatGPT_ID","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");

}

void MainWindow::saveSetting(const QString &key, const QString &value)
{
    QSettings settings("Skly", "SklyChatGPT");
    settings.setValue(key, value);
}


void MainWindow::on_pushButton_clear_clicked()
{
    ui->textBrowserResponse->clear();
}

QString MainWindow::loadSetting(const QString &key, const QString &defaultValue)
{
    // Создайте объект QSettings с указанием имени организации и имени приложения
    QSettings settings("Skly", "SklyChatGPT");

    // Загрузите значение настройки с указанным ключом и значением по умолчанию, если ключ не существует
    return settings.value(key, defaultValue).toString();
}


void MainWindow::on_pushButton_clicked()
{
    settings->show();
}


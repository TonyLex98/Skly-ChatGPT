#include "Settings.h"
#include "ui_Settings.h"
#include <QSettings>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QSettings settings("Skly", "SklyChatGPT");
    ui->lineEditKey->setText( settings.value("yourChatGPT_ID", "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").toString());

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
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_clicked()
{
    emit keyChanged(ui->lineEditKey->text());
}


void Settings::on_pushButton_back_clicked()
{
    this->hide();
}


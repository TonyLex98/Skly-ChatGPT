#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QClipboard>
#include <QMessageBox>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

signals:
    void keyChanged(QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H

#ifndef SELECTBLUETOOTHFORM_H
#define SELECTBLUETOOTHFORM_H


#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
//#include <QTimer>
#include <qmath.h>
#include <vector>
#include "bluetoothmodule.h"
#include "Globals.h"

#include <QString>

#include <QClipboard>
#include <QMessageBox>

namespace Ui {
class SelectBluetoothForm;
}

class SelectBluetoothForm : public QWidget
{
    Q_OBJECT

public:
    explicit SelectBluetoothForm(QWidget *parent = nullptr);
    ~SelectBluetoothForm();


    void on_Connect_bl_clicked();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_find_clicked();

    void on_disconect_clicked();

    void UpdateBluethooth();

    void onReadyRead();

    void socketConnected();

    int GetIndex_of_connectedDevice();

signals:
    void firstWindow();
    void signalInputData(QString str);

private:
    Ui::SelectBluetoothForm *ui;

    int current_bytes_num = 0;
    int all_bytes_num = 0;

    QByteArray current_bytes;
    bool qbaIndutData_global_writeOk = false;

    int index_of_connectedDevice = 0;

};

#endif // SELECTBLUETOOTHFORM_H

#include "bluetooth/selectbluetoothform.h"
#include "ui_selectbluetoothform.h"

SelectBluetoothForm::SelectBluetoothForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectBluetoothForm)
{
    ui->setupUi(this);

    connect(BluetoothModule::getInstance()->socket, &QBluetoothSocket::readyRead, this, &SelectBluetoothForm::onReadyRead);
    connect(BluetoothModule::getInstance()->socket, &QBluetoothSocket::connected, this, &SelectBluetoothForm::socketConnected);

    UpdateBluethooth();

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

SelectBluetoothForm::~SelectBluetoothForm()
{
    delete ui;

}

void SelectBluetoothForm::on_listWidget_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText("Search is complete");
    index_of_connectedDevice =  ui->listWidget->currentRow();
    //qDebug()<< "index_of_connectedDevice: " << index_of_connectedDevice;

    BluetoothModule::getInstance()->deviceSelected(index_of_connectedDevice);
    ui->lineEdit->setText(BluetoothModule::getInstance()->GetNames_of_Connected_Devices().at(index_of_connectedDevice) + " ("
                          + BluetoothModule::getInstance()->GetAddresses_of_Connected_Devices().at(index_of_connectedDevice) + ")");
}

void SelectBluetoothForm::on_Connect_bl_clicked()
{
       //BluetoothModule::getInstance()->startDiscovery();
       if(BluetoothModule::getInstance()->localDevice->isValid()){
            ui->lineEdit->setText("Bluetooth is available on this device");
       }
       else{
            ui->lineEdit->setText("Bluetooth is not available on this device");
       }
}

void SelectBluetoothForm::UpdateBluethooth()
{
    ui->listWidget->clear();
    BluetoothModule::getInstance()->FindConnectedDevice();

    for(int i=0; i<BluetoothModule::getInstance()->GetNames_of_Connected_Devices().size(); i++){
        ui->listWidget->addItem(BluetoothModule::getInstance()->GetNames_of_Connected_Devices().at(i) + " (" + BluetoothModule::getInstance()->GetAddresses_of_Connected_Devices().at(i) + ")");
    }
}

void SelectBluetoothForm::on_find_clicked()
{
    if(BluetoothModule::getInstance()->localDevice->isValid() ){

        UpdateBluethooth();
        ui->lineEdit->setText("Search complete");
    }
    else
    {
        //qDebug() << "Not valid!";
    }
}

void SelectBluetoothForm::on_disconect_clicked()
{
    this->hide();
    emit firstWindow();
}

void SelectBluetoothForm::onReadyRead(){

    //Если этот слот сработал, значит в сокет что-то пришло

    //Мониторим, сколько байт подвалило в данный момент
    current_bytes_num = BluetoothModule::getInstance()->socket->bytesAvailable();

     //Считываем все, что пришло
    current_bytes = BluetoothModule::getInstance()->socket->readAll();

    //qDebug()<<"current_bytes = "<<current_bytes;
    //Записываем это все в общий буффер

    qbaIndutData_global.append(current_bytes);

    //добавляем все к предъидущему значению
    all_bytes_num = qbaIndutData_global.size();


    //qDebug() << "Current bytes: " << current_bytes;
    //qDebug() << "All Bytes: " << qbaIndutData_global;
    //qDebug() << "Bytes num all" << all_bytes_num;

    //qDebug()<<"qbaIndutData_global.endsWith('\r') = "<<qbaIndutData_global.endsWith('\r');
    if(qbaIndutData_global.endsWith('\r')){
            emit signalInputData(QString(qbaIndutData_global));
    }

}

void SelectBluetoothForm::socketConnected()
{
    QMessageBox::information(this,"Подключено устройство: ",BluetoothModule::getInstance()->GetNames_of_Connected_Devices().at(index_of_connectedDevice) + " ("
                             + BluetoothModule::getInstance()->GetAddresses_of_Connected_Devices().at(index_of_connectedDevice) + ")");

}

int SelectBluetoothForm::GetIndex_of_connectedDevice()
{
    return index_of_connectedDevice;
}



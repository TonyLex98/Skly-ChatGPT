#ifndef BLUETOOTHMODULE_H
#define BLUETOOTHMODULE_H
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothServiceDiscoveryAgent>
#include <QMainWindow>
#include <QObject>
#include <QBluetoothSocket>
#include <QBluetoothAddress>
//#include <QDebug>
#include <QBluetoothServiceInfo>
#include <QByteArray>
#include <QTimer>
#include <QEventLoop>
#include <QJniEnvironment>


class BluetoothModule: public QObject
{
   Q_OBJECT
public:

    static BluetoothModule * getInstance();

    QBluetoothAddress selecDevAdress ;
    BluetoothModule();
    ~BluetoothModule();
    BluetoothModule(QObject *parent);
    void startDiscovery(); //

    QBluetoothLocalDevice* localDevice;
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothSocket* socket;

    void deviceSelected(int i);

    void FindConnectedDevice();
    QList<QString> GetNames_of_Connected_Devices();
    QList<QString> GetAddresses_of_Connected_Devices();

    void socketRead();

    void sendData(const QByteArray& data);



private:
    static BluetoothModule * m_singletone;
    QList<QString> names_of_Connected_Devices;
    QList<QString> addresses_of_Connected_Devices;
    QTimer *send_timer;
    QByteArray byte_array;
    int byte_array_index;

private slots:
    //void socketConnected();
    void socketError(QBluetoothSocket::SocketError socketerror);
    void slot_timeout();





};


#endif // BLUETOOTHMODULE_H

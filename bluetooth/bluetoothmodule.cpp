#include "bluetooth/bluetoothmodule.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>

#include <QJniObject>

BluetoothModule * BluetoothModule::m_singletone = nullptr;

BluetoothModule * BluetoothModule::getInstance()
{
    if (!m_singletone)
        m_singletone = new BluetoothModule;

    return m_singletone;
}


BluetoothModule::BluetoothModule(){

    send_timer = new QTimer();
    localDevice = new QBluetoothLocalDevice(this);
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol,this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    //connect(socket, &QBluetoothSocket::readyRead, this, &Device::on_readyRead);
    //connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(on_error(QBluetoothSocket::SocketError)));
    //connect(socket, &QBluetoothSocket::errorOccurred, this, &BluetoothModule::on_error);

    //connect(BluetoothModule::getInstance()->socket, &QBluetoothSocket::connected, this, &BluetoothModule::socketConnected);

    connect(send_timer, &QTimer::timeout, this, &BluetoothModule::slot_timeout);

}

BluetoothModule::~BluetoothModule(){

    delete localDevice;
    delete socket;
    delete discoveryAgent;

}



void BluetoothModule::deviceSelected(int i)

{
    socket->connectToService(QBluetoothAddress(addresses_of_Connected_Devices.at(i)), QBluetoothUuid(QLatin1String("00001101-0000-1000-8000-00805F9B34FB")));
}


void BluetoothModule::FindConnectedDevice()
{
    names_of_Connected_Devices.clear();
    addresses_of_Connected_Devices.clear();

    QJniObject jniBluetoothAdapter = QJniObject::callStaticObjectMethod("android/bluetooth/BluetoothAdapter", "getDefaultAdapter", "()Landroid/bluetooth/BluetoothAdapter;");

    if (jniBluetoothAdapter.isValid())
    {
        QJniObject jniBondedDevicesSet = jniBluetoothAdapter.callObjectMethod("getBondedDevices", "()Ljava/util/Set;");

        if (jniBondedDevicesSet.isValid())
        {
            QJniObject jniIterator = jniBondedDevicesSet.callObjectMethod("iterator", "()Ljava/util/Iterator;");

            if (jniIterator.isValid())
            {
                while(true)
                {
                    jboolean hasNext = jniIterator.callMethod<jboolean>("hasNext");

                    if (!hasNext)
                        break;

                    QJniObject jniBluetoothDevice = jniIterator.callObjectMethod("next", "()Ljava/lang/Object;");
                    QJniEnvironment jniEnv;

                    if (jniEnv->ExceptionCheck())
                    {
                        jniEnv->ExceptionClear();
                        break;
                    }

                    if (jniBluetoothDevice.isValid())
                    {
                        QJniObject jniDeviceName    = jniBluetoothDevice.callObjectMethod("getName", "()Ljava/lang/String;");
                        QJniObject jniDeviceAddress = jniBluetoothDevice.callObjectMethod("getAddress", "()Ljava/lang/String;");

                        if (jniDeviceName.isValid() && jniDeviceAddress.isValid())
                        {
                            const QString device = jniDeviceName.toString();
                            const QString address = jniDeviceAddress.toString();
                            names_of_Connected_Devices.append(device);
                            addresses_of_Connected_Devices.append(address);
                            // Save device and address somewhere
                        }
                    }
                }
            }
        }

        // return result here
    }
}
/*
void BluetoothModule::FindConnectedDevice()
{
    names_of_Connected_Devices.clear();
    addresses_of_Connected_Devices.clear();

    QAndroidJniObject jniBluetoothAdapter = QAndroidJniObject::callStaticObjectMethod("android/bluetooth/BluetoothAdapter", "getDefaultAdapter", "()Landroid/bluetooth/BluetoothAdapter;");

    if (jniBluetoothAdapter.isValid())
    {
        QAndroidJniObject jniBondedDevicesSet = jniBluetoothAdapter.callObjectMethod("getBondedDevices", "()Ljava/util/Set;");

        if (jniBondedDevicesSet.isValid())
        {
            QAndroidJniObject jniIterator = jniBondedDevicesSet.callObjectMethod("iterator", "()Ljava/util/Iterator;");

            if (jniIterator.isValid())
            {
                forever
                {
                    jboolean hasNext = jniIterator.callMethod<jboolean>("hasNext");

                    if (!hasNext)
                        break;

                    QAndroidJniObject jniBluetoothDevice = jniIterator.callObjectMethod("next", "()Ljava/lang/Object;");
                    QAndroidJniEnvironment jniEnv;

                    if (jniEnv->ExceptionCheck())
                    {
                        jniEnv->ExceptionClear();
                        break;
                    }

                    if (jniBluetoothDevice.isValid())
                    {
                        QAndroidJniObject jniDeviceName    = jniBluetoothDevice.callObjectMethod("getName", "()Ljava/lang/String;");
                        QAndroidJniObject jniDeviceAddress = jniBluetoothDevice.callObjectMethod("getAddress", "()Ljava/lang/String;");

                        if (jniDeviceName.isValid() && jniDeviceAddress.isValid())
                        {
                            const QString device = jniDeviceName.toString();
                            const QString address = jniDeviceAddress.toString();
                            names_of_Connected_Devices.append(device);
                            addresses_of_Connected_Devices.append(address);
                            // Save device and address somewhere
                        }
                    }
                }
            }
        }

        // return result here
    }

}
*/
QList<QString> BluetoothModule::GetNames_of_Connected_Devices()
{
    return names_of_Connected_Devices;
}

QList<QString> BluetoothModule::GetAddresses_of_Connected_Devices()
{
    return addresses_of_Connected_Devices;
}

void BluetoothModule::sendData(const QByteArray& data)
{
    byte_array.clear();
    byte_array = data;
    byte_array_index = 0;
    send_timer->start(100);
}

void BluetoothModule::socketError(QBluetoothSocket::SocketError socketerror)
{
    //qDebug() << "Bluetooth error:" << socket->errorString();

}

void BluetoothModule::slot_timeout()
{
    if(byte_array_index<byte_array.size()){
        QByteArray block;
        block.append(byte_array[byte_array_index]);
        socket->write(block);
        byte_array_index++;
    } else{
        send_timer->stop();
    }
}

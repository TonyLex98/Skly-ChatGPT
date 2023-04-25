QT       += core gui
QT += network
QT += bluetooth
QT += core-private
CONFIG += mobility
MOBILITY = connectivity

#QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Settings.cpp \
    bluetooth/Globals.cpp \
    bluetooth/bluetoothmodule.cpp \
    bluetooth/selectbluetoothform.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Settings.h \
    bluetooth/Globals.h \
    bluetooth/bluetoothmodule.h \
    bluetooth/selectbluetoothform.h \
    mainwindow.h

FORMS += \
    Settings.ui \
    bluetooth/selectbluetoothform.ui \
    mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android: include(./android_openssl/openssl.pri)



#android {
 #   ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
#}

RESOURCES += \
    Image.qrc

DISTFILES += \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

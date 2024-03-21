#-------------------------------------------------
#
# Project created by QtCreator 2020-03-26T09:18:41
#
#-------------------------------------------------

QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeviceInfo
TEMPLATE = app

RC_ICONS = image/logo.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    src/common.cpp \
    src/mainwindow.cpp \
    src/myclient.cpp \
    src/mydialog.cpp \
    src/mytable.cpp \
    src/mytableitem.cpp \
    src/sethidedialog.cpp \
    src/setting.cpp \
    src/udpthread.cpp

HEADERS += \
    src/common.h \
    src/mainwindow.h \
    src/myclient.h \
    src/mydialog.h \
    src/mytable.h \
    src/mytableitem.h \
    src/sethidedialog.h \
    src/setting.h \
    src/udpthread.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

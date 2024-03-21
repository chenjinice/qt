#-------------------------------------------------
#
# Project created by QtCreator 2019-08-05T09:30:21
#
#-------------------------------------------------

QT       += core gui webkitwidgets
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mapTool
TEMPLATE = app

SOURCES += main.cpp\
    protobuf/vision.pb.cc \
    qtwidget/logdock.cpp \
    qtwidget/mainwindow.cpp \
    qtwidget/zmqdock.cpp \
    qtwidget/webkit/mykit.cpp \
    qtwidget/webkit/myview.cpp \
    qtwidget/mydialog.cpp \
    protobuf/fusion.pb.cc \
    qtwidget/setting.cpp \
    protobuf/radar.pb.cc

HEADERS  += \
    qtwidget/logdock.h \
    qtwidget/mainwindow.h \
    qtwidget/zmqdock.h \
    qtwidget/webkit/mykit.h \
    qtwidget/webkit/myview.h \
    qtwidget/mydialog.h \
    protobuf/fusion.pb.h \
    qtwidget/setting.h \
    protobuf/radar.pb.h


LIBS += -L$$PWD/libs/lib/ -lzmq
LIBS += -L$$PWD/libs/lib/ -lprotobuf
LIBS += -lpthread

INCLUDEPATH += qtwidget
INCLUDEPATH += qtwidget/webkit
INCLUDEPATH += protobuf
INCLUDEPATH += libs/include

RESOURCES += \
    image/resource.qrc








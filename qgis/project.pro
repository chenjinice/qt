#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T16:03:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gis/giswindow.cpp \
    gis/mytree.cpp \
    gis/mylist.cpp \
    gis/mapconfig.cpp

HEADERS += \
        mainwindow.h \
    gis/giswindow.h \
    gis/mytree.h \
    gis/mylist.h \
    gis/mapconfig.h

# qgis need ==>

# c++11
QMAKE_CXXFLAGS += -std=c++11

QT       += xml
unix{
    DEFINES += CORE_EXPORT=
    DEFINES += GUI_EXPORT=
    DEFINES += ANALYSIS_EXPORT=
    INCLUDEPATH += /usr/include/qgis/
    LIBS += -L/usr/lib/qgis/ -lqgis_gui -lqgis_core
}
!unix{
    DEFINES += CORE_EXPORT=__declspec(dllimport)
    DEFINES += GUI_EXPORT=__declspec(dllimport)
    DEFINES += ANALYSIS_EXPORT=__declspec(dllimport)
}
# qgis need <==

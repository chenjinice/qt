#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T10:58:28
#
#-------------------------------------------------

QT       += core gui multimedia
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = play
TEMPLATE = app


SOURCES += main.cpp \
    player/player.cpp \
    player/plugin/clog.cpp \
    player/plugin/myslider.cpp \
    player/file/decoderfile.cpp \
    player/file/video.cpp \
    player/file/audio.cpp \
    player/udp/decoderudp.cpp \
    player/udp/audioudp.cpp \
    player/udp/videoudp.cpp

HEADERS  += \
    player/player.h \
    player/decoder.h \
    player/plugin/clog.h \
    player/plugin/myslider.h \
    player/file/decoderfile.h \
    player/file/video.h \
    player/file/audio.h \
    player/udp/decoderudp.h \
    player/udp/audioudp.h \
    player/udp/videoudp.h


INCLUDEPATH += /usr/local/ffmpeg/include
LIBS += -L/usr/local/ffmpeg/lib -lavdevice -lavfilter -lavformat -lavcodec -lswscale -lswresample  -lavutil

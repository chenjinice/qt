

CONFIG   += c++11


TARGET = sender

SOURCES += \
    main.cpp \
    server.cpp \
    ../protobuf/fusion.pb.cc \
    ../protobuf/vision.pb.cc \
    ../protobuf/radar.pb.cc



LIBS += -L../libs/lib/ -lzmq
LIBS += -L../libs/lib/ -lprotobuf
LIBS += -lpthread


INCLUDEPATH += ../protobuf
INCLUDEPATH += ../libs/include

HEADERS += \
    server.h \



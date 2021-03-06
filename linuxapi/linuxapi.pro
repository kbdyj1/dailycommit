TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

macx {
    DEFINES += OS_MAC
}

SOURCES += \
        ch10.cpp \
        ch11.cpp \
        ch12.cpp \
        ch13.cpp \
        ch14.cpp \
        ch15.cpp \
        ch16.cpp \
        ch5.cpp \
        ch6.cpp \
        ch7.cpp \
        ch8.cpp \
        ch9.cpp \
        main.cpp \
        utils.cpp

HEADERS += \
    utils.h

DEFINES += _GNU_SOURCE

LIBS += -lcrypt

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ch5.cpp \
        ch6.cpp \
        ch7.cpp \
        main.cpp \
        utils.cpp

HEADERS += \
    utils.h

DEFINES += _GNU_SOURCE

LIBS += -lcrypt

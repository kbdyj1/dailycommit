TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ch_17.cpp \
        ch_20.cpp \
        ch_21.cpp \
        main.cpp \
        utils.cpp

LIBS += -lrt -lcrypt

HEADERS += \
    utils.h

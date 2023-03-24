TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ch_17.cpp \
        ch_20.cpp \
        ch_21.cpp \
        ch_22.cpp \
        ch_23.cpp \
        ch_24.cpp \
        ch_25.cpp \
        ch_26.cpp \
        ch_27.cpp \
        ch_28.cpp \
        ch_29.cpp \
        ch_30.cpp \
        ch_31.cpp \
        ch_32.cpp \
        main.cpp \
        utils.cpp

LIBS += -lrt -lcrypt -lpthread

HEADERS += \
    utils.h

DEFINES += "_POSIX_C_SOURCE=200112"

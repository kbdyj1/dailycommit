TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        1/ch_17.cpp \
        1/ch_20.cpp \
        1/ch_21.cpp \
        1/ch_22.cpp \
        1/ch_23.cpp \
        1/ch_24.cpp \
        1/ch_25.cpp \
        1/ch_26.cpp \
        1/ch_27.cpp \
        1/ch_28.cpp \
        1/ch_29.cpp \
        1/ch_30.cpp \
        1/ch_31.cpp \
        1/ch_32.cpp \
        1/ch_33.cpp \
        1/ch_34.cpp \
        1/ch_35.cpp \
        1/ch_37.cpp \
        2/ch2_01.cpp \
        2/ch2_02.cpp \
        2/ch2_03.cpp \
        2/ch2_04.cpp \
        2/ch2_05.cpp \
        2/ch2_07.cpp \
        2/ch2_08.cpp \
        2/ch2_09.cpp \
        2/ch2_10.cpp \
        2/ch2_11.cpp \
        2/ch2_12.cpp \
        2/ch2_13.cpp \
        2/ch2_15.cpp \
        2/ch2_16.cpp \
        2/ch2_17.cpp \
        2/ch2_18.cpp \
        2/ch2_19.cpp \
        2/ch2_20.cpp \
        2/ch2_22.cpp \
        main.cpp \
        mysock.cpp \
        utils.cpp

LIBS += -lrt -lcrypt -lpthread -lcap -ldl

HEADERS += \
    mysock.h \
    semun.h \
    utils.h

DEFINES += "_POSIX_C_SOURCE=200112"

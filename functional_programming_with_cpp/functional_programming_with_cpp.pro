TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

INCLUDEPATH += range-v3/include

SOURCES += \
        ch_01.cpp \
        ch_02.cpp \
        ch_03.cpp \
        ch_04.cpp \
        ch_05.cpp \
        ch_06.cpp \
        ch_08.cpp \
        ch_09.cpp \
        ch_10.cpp \
        ch_11.cpp \
        ch_12.cpp \
        ch_13.cpp \
        ch_14.cpp \
        ch_15.cpp \
        main.cpp \
        parts/doctest.cpp

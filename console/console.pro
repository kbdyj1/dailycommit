TARGET = console

QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

INCLUDEPATH += ./include \
    ./include/kdbindings

SOURCES = main.cpp \
        src/test_stringview.cpp \
        src/test_kdbindings.cpp

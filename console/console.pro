TARGET = console

QT -= gui
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

INCLUDEPATH += ./include \
    ./include/kdbindings

SOURCES = main.cpp \
        src/test_network.cpp \
        src/test_qtcontainer.cpp \
        src/test_stringview.cpp \
        src/test_kdbindings.cpp

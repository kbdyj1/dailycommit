TARGET = console

QT -= gui
QT += network concurrent

CONFIG += c++2a console
CONFIG -= app_bundle

INCLUDEPATH += ./include \
    ./include/kdbindings

SOURCES = main.cpp \
        src/test_concurrent.cpp \
        src/test_cxx2a.cpp \
        src/test_network.cpp \
        src/test_qtcontainer.cpp \
        src/test_stringview.cpp \
        src/test_kdbindings.cpp

HEADERS +=

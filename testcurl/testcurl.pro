QT -= gui
QT += network

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

LIBS += -lcurl

SOURCES += \
        main.cpp \
        test_non_qt.cpp \
        test_qt.cpp

HEADERS += \
    TestCurl.h

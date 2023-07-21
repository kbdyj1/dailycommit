QT -= gui
QT += network

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

LIBS += -lcurl

SOURCES += \
        Curl.cpp \
        main.cpp \
        test_non_qt.cpp \
        test_non_qt_class.cpp \
        test_qt.cpp

HEADERS += \
    Curl.h \
    TestCurl.h

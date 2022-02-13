TARGET = console

QT -= gui
QT += network concurrent sql

CONFIG += c++2a console
CONFIG -= app_bundle

INCLUDEPATH += ./include \
    ./include/kdbindings

SOURCES = main.cpp \
        src/test_concurrent.cpp \
        src/test_cxx17.cpp \
        src/test_cxx2a.cpp \
        src/test_iodevice.cpp \
        src/test_json.cpp \
        src/test_locale.cpp \
        src/test_network.cpp \
        src/test_qtcontainer.cpp \
        src/test_sql.cpp \
        src/test_stringview.cpp \
        src/test_kdbindings.cpp \
        src/test_xml.cpp

HEADERS +=

RESOURCES += \
    console.qrc

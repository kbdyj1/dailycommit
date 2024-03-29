TEMPLATE = app
CONFIG += console -std=c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        test_cin.cpp \
        test_fotify_source.c \
        test_gets.c \
        test_memstream.c \
        test_strcpy.cpp \
        test_string_out_of_range.cpp

HEADERS +=

QMAKE_CXXFLAGS += -O2   #enable __builtin_object_size
QMAKE_CXXFLAGS += -D_FORTIFY_SOURCE=2

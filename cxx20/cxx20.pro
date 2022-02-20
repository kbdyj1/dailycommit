TEMPLATE = app
CONFIG += console c++2a -fcoroutines
CONFIG -= app_bundle
CONFIG -= qt

unix:!macx {
}
macx {
    DEFINES += OS_MAC
}

SOURCES += \
        main.cpp \
        test_ch1.cpp \
        test_ch3.cpp

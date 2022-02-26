TEMPLATE = app
CONFIG += console c++2a -fcoroutines
#CONFIG += console c++20 -fmodules-ts
CONFIG -= app_bundle
CONFIG -= qt

unix:!macx {
}
macx {
    DEFINES += OS_MAC
}

#DEFINES += USE_CXX20_FEATURE_MODULE

SOURCES += \
        main.cpp \
        test_ch1.cpp \
        test_ch3.cpp \
        test_ch3_2_module.cpp

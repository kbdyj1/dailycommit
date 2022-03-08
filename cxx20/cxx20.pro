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
        test_ch3_2_module.cpp \
        test_ch3_3_3way_comparision.cpp \
        test_ch3_4_initialize.cpp \
        test_ch3_5_const.cpp \
        test_ch3_5_const_init_order.cpp \
        test_ch3_6_template_improve.cpp \
        test_ch3_7_lambda.cpp \
        test_ch3_8_attributes.cpp \
        test_ch3_9_etc.cpp \
        test_ch4_2_span.cpp \
        test_ch4_3_container.cpp

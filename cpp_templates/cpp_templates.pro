TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ch_01.cpp \
        ch_01_max.cpp \
        ch_02.cpp \
        ch_03.cpp \
        ch_04.cpp \
        ch_05.cpp \
        ch_06.cpp \
        ch_06_enable_if.cpp \
        ch_06_perfect_forwarding.cpp \
        ch_07.cpp \
        ch_08.cpp \
        ch_08_constexpr.cpp \
        ch_08_meta_programming.cpp \
        ch_08_sfinae.cpp \
        ch_09.cpp \
        ch_10.cpp \
        ch_11.cpp \
        ch_11_forward.cpp \
        ch_11_function.cpp \
        ch_11_util.cpp \
        ch_12.cpp \
        ch_12_friend.cpp \
        ch_12_template_parameter.cpp \
        ch_12_template_parameter_pack.cpp \
        ch_13.cpp \
        ch_13_adl.cpp \
        ch_13_dependency.cpp \
        ch_13_hack.cpp \
        ch_13_inherit.cpp \
        ch_14.cpp \
        ch_14_instance_model.cpp \
        ch_14_lazy.cpp \
        ch_15.cpp \
        ch_15_auto.cpp \
        ch_15_limit.cpp \
        ch_15_rvalue.cpp \
        ch_15_sfinae.cpp \
        main.cpp

HEADERS += \
    ch_14_t.hpp \
    stack.hpp

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        test_assert.cpp \
        test_constexpr.cpp \
        test_emplaceback.cpp \
        test_exception.cpp \
        test_fileio.cpp \
        test_lambda.cpp \
        test_macro.cpp \
        test_memalign.cpp \
        test_move.cpp \
        test_vtemplate.cpp \
        test_weak_ptr.cpp

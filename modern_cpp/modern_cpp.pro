TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ch_01.cpp \
        ch_01_lib.cpp \
        ch_01_structured_binding.cpp \
        ch_02.cpp \
        ch_02_literals.cpp \
        ch_02_random.cpp \
        ch_02_regexp.cpp \
        ch_02_string.cpp \
        ch_03.cpp \
        ch_03_highorder.cpp \
        ch_03_invoke.cpp \
        ch_03_lambda.cpp \
        ch_03_vargs.cpp \
        ch_04.cpp \
        ch_04_attribute.cpp \
        ch_04_static_assert.cpp \
        ch_04_stringfy.cpp \
        ch_05.cpp \
        ch_05_algorithm.cpp \
        ch_05_iterator.cpp \
        ch_06.cpp \
        ch_06_any.cpp \
        ch_06_chrono.cpp \
        ch_06_exit_handler.cpp \
        ch_06_hash.cpp \
        ch_06_type.cpp \
        ch_07.cpp \
        ch_07_file.cpp \
        ch_07_filesystem.cpp \
        ch_07_stream.cpp \
        ch_08.cpp \
        ch_08_condition.cpp \
        ch_08_lock.cpp \
        ch_08_promise.cpp \
        ch_08_thread.cpp \
        main.cpp

DISTFILES +=

HEADERS += \
    MyArray.h \
    util.h

LIBS += -lpthread

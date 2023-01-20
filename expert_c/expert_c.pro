TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ch_01.c \
        ch_05.c \
        ch_06.c \
        ch_08.c \
        ch_08_person.c \
        ch_08_student.c \
        ch_11.c \
        ch_15.c \
        ch_16.c \
        ch_17.c \
        ch_17_shm.c \
        ch_17_shm_compete.c \
        ch_18.c \
        ch_18_cancel_flag.c \
        ch_18_mutex.c \
        ch_18_shared_cond.c \
        ch_18_shared_int32.c \
        ch_18_shared_mem.c \
        ch_18_shared_mutex.c \
        ch_18_test.c \
        ch_19.c \
        ch_19_msgq.c \
        ch_19_pipe.c \
        main.c

HEADERS += \
    ch_08_person.h \
    ch_08_person_p.h \
    ch_08_student.h \
    ch_18_shared_cond.h \
    ch_18_shared_int32.h \
    ch_18_shared_mem.h \
    ch_18_shared_mutex.h \
    common_def.h \
    helper.h

LIBS += -lpthread -lrt

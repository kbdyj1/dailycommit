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
        ch_18_mutex.c \
        main.c

HEADERS += \
    ch_08_person.h \
    ch_08_person_p.h \
    ch_08_student.h \
    helper.h

LIBS += -lpthread -lrt

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    big2.h

DISTFILES += \
    libBig.so

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lBig
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lBig
else:unix: LIBS += -L$$PWD/./ -lBig

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

QT -= gui
QT += network

CONFIG += c++2a console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        #TestExec.cpp \
        main.cpp \
        test_bytearray.cpp \
        test_cbor.cpp \
        test_datetime.cpp \
        test_fileinfo.cpp \
        test_implcitsharing.cpp \
        test_ioprio.cpp \
        test_jason.cpp \
        test_network.cpp \
        test_ownership.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    #TestExec.h

#LIBS += -lssl -lcrypto

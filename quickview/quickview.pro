TARGET = quickview

QT += quick qml quick3d statemachine

QMLS = ./qml
SRC_DIR = ./src
HDR_DIR = ./include

INCLUDEPATH += $$HDR_DIR

SOURCES += \
   main.cpp \
   $$SRC_DIR/invokeMethod.cpp \
   $$SRC_DIR/nestedItem.cpp \
   $$SRC_DIR/test_newQmlGrammer.cpp \
   $$SRC_DIR/test_quick3d.cpp \
   $$SRC_DIR/test_stateMachine.cpp \
   $$SRC_DIR/test_jsvalue.cpp \
   src/test_jsengine.cpp

HEADERS += \
    $$HDR_DIR/Actor.h \
    $$HDR_DIR/Rectangle.h

resources.files = \
    $$QMLS/main.qml \
    $$QMLS/nested.qml \
    $$QMLS/NullishCoalescing.qml \
    $$QMLS/AccessbilityButton.qml \
    $$QMLS/UseNewQmlLanguageFeature.qml \
    $$QMLS/Images.qml \
    $$QMLS/simple3d.qml \
    $$QMLS/TestStateMachine.qml

resources.prefix = /$${TARGET}
RESOURCES += resources \
    dailycommit.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \


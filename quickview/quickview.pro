TARGET = quickview

QT += quick qml quick3d statemachine webview networkauth

QMLS = ./qml
JS_DIR = ./js
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
   src/test_defaultProperty.cpp \
   src/test_http.cpp \
   src/test_javascript.cpp \
   src/test_jsengine.cpp \
   src/test_localStorage.cpp \
   src/test_oauth2.cpp \
   src/test_simpleQml.cpp

HEADERS += \
    $$HDR_DIR/Actor.h \
    $$HDR_DIR/Rectangle.h \
    include/BirthdayParty.h \
    include/Person.h

resources.files = \
    qml/main.qml \
    qml/nested.qml \
    qml/NullishCoalescing.qml \
    qml/AccessbilityButton.qml \
    qml/UseNewQmlLanguageFeature.qml \
    qml/Images.qml \
    qml/simple3d.qml \
    qml/TestStateMachine.qml \
    qml/MyButton.qml \
    qml/HttpWindow.qml \
    qml/LocalStorageSample.qml \
    qml/TestInput.qml \
    qml/AttachedPropertiesAndSignalHandlers.qml \
    qml/TestDefaultProperty.qml \
    qml/TestQuick.qml \
    qml/TestAnimation.qml \
    qml/my_button_impl.js \
    qml/factorial.js \
    qml/common_button_impl.js \
    qml/RtoL.qml

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














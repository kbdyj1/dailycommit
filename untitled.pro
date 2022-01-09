QT += quick qml

SOURCES += \
   main.cpp \
   invokeMethod.cpp \
   nestedItem.cpp

HEADERS += \
    Actor.h \
    Rectangle.h

resources.files = \
    main.qml \
    nested.qml

resources.prefix = /$${TARGET}
RESOURCES += resources

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

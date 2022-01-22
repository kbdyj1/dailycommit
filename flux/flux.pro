QT += quick

SOURCES += \
        main.cpp

resources.files = \
        qml/main.qml  \
        qml/actions/AppActions.qml \
        qml/dispatcher/Dispatcher.qml \
        qml/stores/MainStore.qml \
        qml/stores/SubStore.qml \
        qml/views/MainView.qml \
        qml/views/SubView.qml

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

DISTFILES += \


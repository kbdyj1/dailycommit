QT += quick qml quick3d

KDBINDINGS = ./kdbindings

INCLUDEPATH += $$KDBINDINGS

SOURCES += \
   main.cpp \
   invokeMethod.cpp \
   nestedItem.cpp \
   test_kdbindings.cpp \
   test_newQmlGrammer.cpp \
   test_quick3d.cpp \
   test_stringview.cpp

HEADERS += \
    Actor.h \
    Rectangle.h \
    binding.h \
    binding_evaluator.h \
    genindex_array.h \
    make_node.h \
    node.h \
    node_functions.h \
    node_operators.h \
    property.h \
    property_updater.h \
    signal.h \
    utils.h

resources.files = \
    main.qml \
    nested.qml \
    NullishCoalescing.qml \
    AccessbilityButton.qml \
    UseNewQmlLanguageFeature.qml \
    Images.qml \
    simple3d.qml

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

DISTFILES +=

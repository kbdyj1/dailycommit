import QtQuick 2.15
import QtQuick.Window 2.15
import toy.core 1.0

Window {
    id: window

    width: 480
    height: 640
    visible: true
    title: qsTr("Hello World")

    Item {
        id: root

        width: window.width
        height: window.height

        Component.onCompleted: {
            dispatcher.dispatch(Action.RegisterRoot, {'root':root})
            dispatcher.dispatch(Action.CreateView, {'viewName':'Login', 'args':{}})
        }
    }
}

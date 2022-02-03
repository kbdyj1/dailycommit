import QtQuick 2.15
import QtQuick.Window 2.15
import toy.core 1.0

Window {
    id: window

    width: 480
    height: 640
    visible: true
    title: qsTr("Hello World")

    function createSystemAction(action, param) {
        dispatcher.dispatch(action, param)
    }

    Item {
        id: root

        width: window.width
        height: window.height

        Component.onCompleted: {
            createSystemAction(Action.RegisterRoot, {'root':root})
            createSystemAction(Action.CreateView, {'viewName':'Login', 'args':{}})
        }
    }
}

import QtQuick

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Light (D-Bus)")

    Rectangle {
        width: window.width
        height: window.height
        color: light.on ? "lightgrey" : "black"
    }
}

import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: window

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        width: window.width; height: window.height
        color: "blue"

        RedBox {
            id: redBox

            width: parent.width / 2
            height: parent.height / 2
            anchors.centerIn: parent
        }
        Circle {
            width: 32; height: 32
        }
    }
}

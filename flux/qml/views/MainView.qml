import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root

    property alias notiCountText: text.text

    signal buttonPressed()
    signal notiPressed()

    Rectangle {
        id: noti

        width: 16; height: 16; radius: 8
        color: "red"
        Text {
            id: text
            color: "white"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: root.notiPressed()
        }
    }
    Button {
        anchors.top: noti.bottom
        width: 128; height: 32
        text: "push"
        onClicked: root.buttonPressed()
    }
}

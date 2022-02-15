import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root

    width: 640; height: 480

    Rectangle {
        anchors.fill: parent
        color: "yellow"

        Rectangle {
            id: rect

            anchors.centerIn: parent
            width: 100
            height: width
            color: "red"
            focus: true

            function incSize() {
                rect.width += 10
            }
            function decSize() {
                rect.width -= 10
            }

//            TapHandler {
//                onTapped: rect.width += 10
//            }
            MouseArea {
                anchors.fill: parent
                onClicked: incSize()
            }
            Keys.onUpPressed: incSize()
            Keys.onDownPressed: decSize()
        }
    }
}

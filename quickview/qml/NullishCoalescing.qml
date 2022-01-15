import QtQuick 2.0

Item {
    property var settingColor

    width: 256; height: 256

    Rectangle {
        anchors.fill: parent
        color: settingColor ?? "blue"
    }
}

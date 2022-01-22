import QtQuick 2.0

Item {
    id: root

    height: 24

    property alias message: text.text

    onMessageChanged: {
        rect.y = height

        showAnimation.restart()
        hideAnimation.stop()
        timer.restart()
    }

    Timer {
        id: timer

        interval: 2500
        onTriggered: hideAnimation.start()
    }

    NumberAnimation {
        id: showAnimation

        target: rect
        property: "y"
        to: -2
        duration: 500
        easing.type: Easing.OutBack
    }
    NumberAnimation {
        id: hideAnimation

        target: rect
        property: "y"
        to: height
        duration: 500
        easing.type: Easing.InBack
    }

    Rectangle {
        id: rect

        anchors.left: parent.left; anchors.right: parent.right
        anchors.margins: 2
        height: parent.height
        y: parent.height
        radius: 4
        border.width: 1; border.color: 'lightgrey'

        color: "lightyellow"

        Text {
            id: text
            x: 8
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

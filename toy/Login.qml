import QtQuick 2.0
import Login 1.0

View {
    anchors.fill: parent

    property alias title: text.text

    Component.onCompleted: {
        console.log("Login.onCompleted")
        createAction(Login.Created, {})
    }

    Rectangle {
        anchors.fill: parent
        color: 'lightsteelblue'

        Text {
            id: text
            y: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

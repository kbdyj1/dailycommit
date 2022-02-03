import QtQuick 2.0
import QtQuick.Controls 2.0
import Login 1.0

View {
    anchors.fill: parent

    property alias title: text.text

    Component.onCompleted: {
        console.log("Login.onCompleted")
        createAction(Login.Created, {})
    }

    component InputBox : Row {
        id: inputBox

        readonly property int defaultTextWidth: 120
        property alias textWidth: label.width
        property alias textLabel: label.text
        property alias textInput: input.text

        width: parent.width
        height: 32

        Text {
            id: label
            width: defaultTextWidth
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }
        TextInput {
            id: input
            width: parent.width - textWidth
            height: parent.height
            verticalAlignment: TextInput.AlignVCenter

            Rectangle {
                anchors.fill: parent
                color: 'transparent'
                border.color: 'lightgray'
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: 'lightsteelblue'

        Text {
            id: text
            y: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Column {
            id: column

            anchors.centerIn: parent

            width: parent.width * (2.0/3.0)

            spacing: 8
            InputBox {
                id: inputId
                textLabel: "id"
            }
            InputBox {
                id: inputPassword
                textLabel: "password"
            }
        }

        Button {
            anchors.left: column.left; anchors.right: column.right
            anchors.top: column.bottom; anchors.topMargin: 32
            height: 32
            text: "Login"

            onClicked: {
                createAction(Login.CheckUser, {'id':inputId.textInput, 'password':inputPassword.textInput})
            }
        }
    }
}

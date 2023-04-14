import QtQuick 2.0
import CppModel.Example 1.0

Item {
    id: root

    anchors.fill: parent

    Rectangle {
        id: divider

        x: parent.width/2
        width: 1
        height: parent.height
        color: "orange"
    }
    ListView {
        id: listVieWithoutModel

        anchors {
            left: parent.left
            right: divider.left
            top: parent.top
            bottom: parent.bottom
        }

        model: CppModel.rawList

        delegate: Text {
            width: ListView.width
            height: 30
            verticalAlignment: Text.AlignVCenter
            text: modelData

            Component.onCompleted: {
                console.log("ListView.delegate created !!!")
            }
            Component.onDestruction: {
                console.log("ListView.delegate destroyed !")
            }
        }
    }
    ListView {
        id: listViewWithModel

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: divider.right
            right: parent.right
        }

        model: CppModel{}

        delegate: Text {
            width: ListView.width
            height: 30
            verticalAlignment: Text.AlignVCenter
            text: model.text

            Component.onCompleted: {
                console.log("Model.delegate created !!!")
            }
            Component.onDestruction: {
                console.log("Model.delegate destroyed !")
            }
        }
    }
}

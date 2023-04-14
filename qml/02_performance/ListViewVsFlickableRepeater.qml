import QtQuick 2.0

Item {
    id: root

    Rectangle {
        id: divider

        x: parent.width/2
        width: 1; height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        color: "orange"
    }
    Flickable {
        id: flickable

        anchors {
            left: parent.left
            right: divider.left
            top: parent.top
            bottom: parent.bottom
        }

        contentHeight: column.height

        Column {
            id: column

            property int createCount: 0

            width: root.width/2

            Repeater {
                id: repeater

                model: 100
                delegate: Rectangle {
                    width: parent.width; height: 30
                    color: "transparent"
                    border.color: "black"
                    Text {
                        anchors.fill: parent
                        text: modelData
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Component.onCompleted: {
                        column.createCount++
                        console.log("Rectangle(Flickable).onCompleted [" + column.createCount + "]")
                    }
                    Component.onDestruction: console.log("Rectangle(Flickable).onDestroyed")
                }
            }
        }
    }
    Rectangle {
        id: scrollBar
        anchors.right: flickable.right
        y: flickable.visibleArea.yPosition * flickable.height
        width: 10
        height: flickable.visibleArea.heightRatio * flickable.height
        color: "orange"
    }

    ListView {
        id: listView

        property int createCount: 0

        anchors {
            left: divider.right; right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        model: 100

        delegate: Rectangle {
            width: parent.width; height: 30
            color: "transparent"
            border.color: "black"
            Text {
                anchors.fill: parent
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Component.onCompleted: {
                listView.createCount++
                console.log("Rectangle(ListView).onCompleted [" + listView.createCount + "]")
            }
            Component.onDestruction: console.log("Rectangle(ListView).onDestroyed")
        }
    }
}

import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    focus: true
    Keys.enabled: true
    Keys.onReturnPressed: {
        console.log("Return key was pressed")
    }

    enum TextType {
        Normal,
        Focused
    }
    ListView {
        id: listview
        width: 240; height: 320
        model: 3
        delegate: Item {
            id: item

            width: 100; height: 30
            Rectangle {
                anchors.fill: parent
                color: item.ListView.isCurrentItem ? "red" : "yellow"
            }
            Text {
                property int textType: item.ListView.isCurrentItem
                                       ? AttachedPropertiesAndSignalHandlers.Focused
                                       : AttachedPropertiesAndSignalHandlers.Normal

                anchors.centerIn: parent
                font.bold: textType === AttachedPropertiesAndSignalHandlers.TextType.Focused
                font.pixelSize: textType === AttachedPropertiesAndSignalHandlers.TextType.Focused ? 24 : 12
                text: index
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listview.currentIndex = index
                }
            }
        }
    }
}

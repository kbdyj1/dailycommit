import QtQuick 2.0

Item {
    id: root
    Component.onCompleted: { console.log("root onCompleted.") }

    Item {
        id: item1
        Component.onCompleted: { console.log("item1 onCompleted.") }

        Item {
            id: item2
            Component.onCompleted: { console.log("item2 onCompleted.") }
        }
    }
}

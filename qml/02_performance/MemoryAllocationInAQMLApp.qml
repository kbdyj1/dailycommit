import QtQuick 2.0

Item {
    id: root

    // r0 and r1 both considered to be of the explicitly defined Rectangle type
    Rectangle {
        id: r0
        color: "red"
    }
    Rectangle {
        id: r1
        color: "blue"
        width: 50
    }

    // implicit types
    Rectangle {
        id: r2
        property int customProperty: 5
    }
    Rectangle {
        id: r3
        property string customProperty: "hello"
    }
    Rectangle {
        id: r4
        property string customProperty: "hello"
    }
}

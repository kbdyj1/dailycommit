import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: window

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    RSSMemoryUtilization {
        width: window.width; height: window.height
    }
}

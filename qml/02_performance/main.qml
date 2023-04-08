import QtQuick

// Use async, event-driven programming
// Use work threads
// Never manually spin the event loop
// Never spend more than a couple of milliseconds per frame within blocking functions

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
}

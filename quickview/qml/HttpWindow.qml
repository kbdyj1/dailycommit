import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: root

    signal startDownloadAt(string url, string path)

    width: 640; height: 480
    color: "lightslategrey"

    /*
     *  inline component
     */
    component LabeledTextInput: Row {
        property alias label: label.text
        property alias input: input.text

        Text {
            id: label
            width: 120; height: 16
        }
        Rectangle {
            height: 16
            width: parent.width - label.width
            color: "lightsteelblue"
            TextInput {
                id: input
                anchors.fill: parent
            }
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        LabeledTextInput {
            id: url
            width: parent.width
            label: 'URL:'
        }
        LabeledTextInput {
            id: downloadDirectory
            width: parent.width
            label: "Download directory:"
        }
    }

    Button {
        anchors.right: parent.right; anchors.rightMargin: 8
        anchors.bottom: parent.bottom; anchors.bottomMargin: 8
        width: 96; height: 32
        text: "Download"
        onClicked: {
            root.startDownloadAt(url.input, downloadDirectory.input)
        }
    }
}

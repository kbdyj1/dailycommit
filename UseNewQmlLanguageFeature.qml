import QtQuick 2.0

Item {
    width: 512; height: 512

    AccessbilityButton {
        id: accessbilityButton

        description: "This is a test button" // requried !!!
        onClicked: (mouse) => {
                       console.log("AccessbilityButton Clicked. [x] -> " + mouse?.x) // optional chaining
                       console.log("AccessbilityButton Clicked. [y] -> " + mouse.y)
                   }

        //onClicked output
        //
        //qml: AccessbilityButton Clicked. [x] -> undefined
        //qrc:/untitled/UseNewQmlLanguageFeature.qml:12: TypeError: Cannot read property 'y' of undefined
    }
    Images {
        anchors.top: accessbilityButton.bottom
        anchors.topMargin: 8
    }
}

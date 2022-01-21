import QtQuick
import './views'
import './dispatcher'
import './stores'
import './actions'

Window {
    id: window

    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    MainView {
        id: view

        width: window.width; height: window.height

        onButtonPressed: {
            actionCreator.createAction('incNoti', 0)
        }
        onNotiPressed: {
            actionCreator.createAction('setNoti', 0)
        }

        notiCountText: mainStore.notiCount
    }
    AppActions {
        id: actionCreator

        dispatcher: dispatcher
    }
    Dispatcher {
        id: dispatcher
    }
    MainStore {
        id: mainStore
        Component.onCompleted: {
            dispatcher.regist(mainStore)
        }
    }
    RootStore {
        id: rootStore
        Component.onCompleted: {
            dispatcher.regist(rootStore)
        }
    }
}

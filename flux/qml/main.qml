import QtQuick
import './views'
import './dispatcher'
import './stores'
import './actions'

Window {
    id: window

    width: 320
    height: 240
    visible: true
    title: qsTr("Hello World")

    Item {
        width: window.width; height: window.height

        //------------------------------------------------------------------------- Views
        MainView {
            id: view

            anchors.fill: parent

            onButtonPressed: {
                actionCreator.createAction('incNoti', 0)
                actionCreator.createAction('showToast', mainStore.notiCount)
            }
            onNotiPressed: {
                actionCreator.createAction('setNoti', 0)
            }

            notiCountText: mainStore.notiCount
        }
        SubView {
            id: toast

            width: parent.width
            anchors.bottom: parent.bottom

            message: subStore.toastMessage
        }
        //------------------------------------------------------------------------- Action
        AppActions {
            id: actionCreator

            dispatcher: dispatcher
        }
        //------------------------------------------------------------------------- Dispatcher
        Dispatcher {
            id: dispatcher
        }
        //------------------------------------------------------------------------- Store
        MainStore {
            id: mainStore
            Component.onCompleted: {
                dispatcher.regist(mainStore)
            }
        }
        SubStore {
            id: subStore
            Component.onCompleted: {
                dispatcher.regist(subStore)
            }
        }
    }
}

import QtQuick 2.0

Item {
    property string toastMessage

    function subscribe(action) {
        switch (action.type) {
        case 'showToast':
            toastMessage = 'you have ' + action.param + ' unread message'
            toastMessage += (action.param <= 1) ? '.' : 's.'
            break
        }
    }
}

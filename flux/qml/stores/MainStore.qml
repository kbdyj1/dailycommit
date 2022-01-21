import QtQuick 2.0

Item {
    property int notiCount

    function subscribe(action) {
        switch (action.type) {
        case 'incNoti':
            notiCount++
            break

        case 'setNoti':
            notiCount = action.param
            break
        }
    }
}

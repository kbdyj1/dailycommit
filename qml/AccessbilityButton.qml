import QtQuick 2.0
import QtQuick.Controls 2.0

Button {
    required property string description
    Accessible.description: description
}

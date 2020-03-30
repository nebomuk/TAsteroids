import QtQuick 2.0

Rectangle {

    property string title1
    width: parent.width
    height: 58
    color: "#00000000"
    radius: 2
    border.color: "#e36873"
    border.width: 2

    Text {
        id : buttonText
        text: title1
        font.pointSize: 24
        font.family: "OCR A Extended"
        color: "#ffe8ea"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter:  parent.horizontalCenter
    }

    MouseArea {

        anchors.fill: parent
        hoverEnabled: Qt.platform.os != "android" && Qt.platform.os != "ios"
        onEntered:  parent.color = parent.border.color
        onExited: parent.color = "#00000000"
    }
}



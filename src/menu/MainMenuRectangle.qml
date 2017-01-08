import QtQuick 2.0

Rectangle {

    property string title1
    width: parent.width
    height: 58
    color: "#00000000"
    radius: 2
    border.color: "#0095a6"
    border.width: 2

    Text {
        id : buttonText
        text: name
        font.pointSize: 24
        font.family: "OCR A Extended"
        color: "#01375D"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter:  parent.horizontalCenter
    }

    MouseArea {

        anchors.fill: parent
        hoverEnabled: Qt.platform.os != "android" && Qt.platform.os != "ios"
        onEntered:  parent.color = parent.border.color
        onExited: parent.color = "#00000000"
    }

    Component.onCompleted: {
        // using title as property did not work as it is already the list delegate's role
        if(buttonText.text === null || buttonText.text === "")
        {
            buttonText.text = title1
        }
    }
}



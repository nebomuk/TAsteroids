import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.LocalStorage 2.0
import QtQuick.Controls 1.4

Window {
    id : rootWindow
    visible: true
    color: "transparent"

    // used for testing on desktop platforms, should be fullscreen anyways
    width:  Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.width : 800
    height: Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.height : 384


    MainFormTouch
    {
            anchors.fill: parent
            newGameMouseArea.onClicked: menuGameInteraction.showGame()
    }
}

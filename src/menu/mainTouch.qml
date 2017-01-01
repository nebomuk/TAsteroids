import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.LocalStorage 2.0
import QtQuick.Controls 1.4

Window {
    id : rootWindow
    visible: true
    color: "transparent"

    flags: Qt.platform.os != "android" && Qt.platform.os != "ios"  ? Qt.FramelessWindowHint | Qt.ToolTip | Qt.WA_TranslucentBackground : ""


    // used for testing on desktop platforms, should be fullscreen anyways
    width:  Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.width : 1020
    height: Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.height : 567


    MainFormTouch
    {
            anchors.fill: parent
            exitMouseArea.onClicked: Qt.quit();
            newGameMouseArea.onClicked: menuGameInteraction.showGame()
            playerCount1MouseArea.onClicked: menuGameInteraction.playerCount = 1;
            playerCount2MouseArea.onClicked: menuGameInteraction.playerCount = 2;
            playerInputVisible: menuGameInteraction.highScore > 0 // when > 0, automatically shows the high score input field
            playerInputHighScore : menuGameInteraction.highScore

    }

    // window dragging
    MouseArea {
        id: dragMouseArea
        z : -1

        enabled : !(Qt.platform.os == "android" || Qt.platform.os == "ios" )
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        property variant clickPos: "1,1"

        // context menu
        onPressed: {
            clickPos  = Qt.point(mouse.x,mouse.y)
            if (mouse.button == Qt.RightButton)
            {
                ctxMenu.popup();
            }
        }

        // context menu

        Menu {
                id: ctxMenu
                MenuItem {
                    text: qsTr("&Quit")
                    onTriggered: Qt.quit();

                }
            }

        // window dragging
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            rootWindow.x += delta.x;
            rootWindow.y += delta.y;
        }
    }
}

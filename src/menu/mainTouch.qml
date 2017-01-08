import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.LocalStorage 2.0
import QtQuick.Controls 1.4

Window {
    id : rootWindow
    visible: true
    color: "transparent"

    flags: Qt.platform.os != "android" && Qt.platform.os != "ios"  ? Qt.FramelessWindowHint | Qt.ToolTip | Qt.WA_TranslucentBackground : ""

    width:  Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.width : 1020
    height: Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.height : 567


    MainFormTouch
    {
            id : mainForm
            anchors.fill: parent

            // show player high score input when a game has finished
            Component.onCompleted:
            {
                menuLevelPlayerInput.highScore =  menuGameInteraction.highScore
                var highScoreVisible = menuGameInteraction.highScore  > 0;
                menuLevelPlayerInput.visible = highScoreVisible
                menuLevelMain.visible = !highScoreVisible
            }

            // android back button handling
            Connections
            {
                target: rootWindow
                onClosing : {
                    if(mainForm.menuLevelMain.visible)
                    {
                        close.accepted = true; // quit app
                    }
                    else
                    {
                        close.accepted = false;
                        mainForm.showMenuLevelMain();
                    }
                }
            }

            btnBackHighScore.onClicked: showMenuLevelMain();
            btnBackPlayerCount.onClicked: showMenuLevelMain();

            // player high score input
            btnConfirm.onClicked: showMenuLevelMain();
            menuLevelPlayerInput.onFinished: showMenuLevelMain();

            btnExit.onClicked: Qt.quit();
            btnNewGame.onClicked:
            {
                if(Qt.platform.os == "android") // only singleplayer on android
                {
                    menuGameInteraction.showGame();
                }
                else
                {
                    mainForm.menuLevelMain.visible = false;
                    mainForm.menuLevelPlayerCount.visible = true;
                }
            }

            btnHighScore.onClicked: {
                mainForm.menuLevelMain.visible = false;
                mainForm.menuLevelHighScore.visible = true;
            }


            btnPlayerCount1.onClicked: {
                menuGameInteraction.playerCount = 1;
                menuGameInteraction.showGame();
            }
            btnPlayerCount2.onClicked: {
                menuGameInteraction.playerCount = 2;
                menuGameInteraction.showGame();
            }

            function showMenuLevelMain()
            {
                mainForm.menuLevelMain.visible = true;
                mainForm.menuLevelPlayerCount.visible = false;
                mainForm.menuLevelPlayerInput.visible = false;
                mainForm.menuLevelHighScore.visible = false;
            }


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

import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import "Style.js" as Style



Item {
    id: item1
//    width: 800
//    height: 384

    property alias menuLevelMain : column
    property alias menuLevelPlayerCount : columnPlayerCount
    property alias menuLevelHighScore : highScoreContainer
    property alias menuLevelPlayerInput : playerInput // uses onFinished
    property alias btnConfirm: btnConfirm
    property alias btnBackPlayerCount: btnBackPlayerCount
    property alias btnBackHighScore: btnBackHighScore
    property alias btnHighScore : btnHighScore
    property alias btnExit : btnExit
    property alias btnNewGame : btnNewGame // mouse event handler outside, starts new game
    property alias btnPlayerCount1: btnPlayerCount1
    property alias btnPlayerCount2: btnPlayerCount2


    property var buttonHeight : image0.height * 0.1
    property var buttonSpacing : image0.height *0.03

    Item
    {
        id: item2

        anchors.left: image0.left
        anchors.right: image1.right // must be right because image1 (middle) gets stretched
        anchors.top: image0.top
        anchors.bottom: image0.bottom

        anchors.topMargin: image0.height * 0.2
        anchors.bottomMargin: image0.height * 0.3
        anchors.leftMargin: image0.width * 0.3
        anchors.rightMargin: -image2.width * 0.1


        // initial main menu items
        Column
        {
            id : column
            anchors.fill: parent
            anchors.topMargin : buttonHeight
            spacing: buttonSpacing


            MainMenuRectangle
            {
                height: buttonHeight
                title1: qsTr("New Game")

                MouseArea
                {
                    id : btnNewGame
                    anchors.fill: parent

                }
            }
            MainMenuRectangle
            {
                height: buttonHeight
                title1: qsTr("High Scores")

                MouseArea
                {
                    id : btnHighScore
                    anchors.fill: parent

                }
            }
            // exit button
            MainMenuRectangle
            {
                height: buttonHeight
                title1: qsTr("Exit")
                visible: Qt.platform.os != "android"

                MouseArea
                {
                    id : btnExit
                    anchors.fill: parent

                }
            }
        }

        Column
        {
            id : columnPlayerCount
            visible: false
            anchors.fill: parent
            anchors.topMargin : buttonHeight
            spacing: buttonSpacing


            MainMenuRectangle
            {
                height: buttonHeight
                title1: qsTr("1 Player")

                MouseArea
                {
                    id : btnPlayerCount1
                    anchors.fill: parent
                }
            }
            MainMenuRectangle
            {
                height: buttonHeight
                title1: qsTr("2 Players")

                MouseArea
                {
                    id : btnPlayerCount2
                    anchors.fill: parent
                }
            }
            // back button
            MainMenuRectangle
            {
                height: buttonHeight
                title1: qsTr("Back To Main Menu")
                visible: Qt.platform.os != "android"

                MouseArea
                {
                    id : btnBackPlayerCount
                    anchors.fill: parent
                }
            }
        }

        PlayerInput
        {
            id : playerInput
            anchors.topMargin : buttonHeight*2
            anchors.top :parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 4
            visible: false
        }

        // confirm button
        MainMenuRectangle
        {
            height: buttonHeight
            visible: playerInput.visible
            anchors.top : playerInput.bottom
            title1: qsTr("Ok")

            MouseArea
            {
                id : btnConfirm
                anchors.fill: parent
            }
        }

        Item
        {
            id : highScoreContainer
            //anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            width : parent.width *0.9
            height: parent.height * 0.9
            visible: false

            HighScoreView2
            {
                id : highscore
                anchors.centerIn : parent
                height : parent.height*0.9
            }
        }

        // back button
        MainMenuRectangle
        {
            height: buttonHeight
            visible: highScoreContainer.visible && Qt.platform.os != "android"
            anchors.top : highScoreContainer.bottom
            title1: qsTr("Back To Main Menu")

            MouseArea
            {
                id : btnBackHighScore
                anchors.fill: parent
            }
        }
    }


    AsteroidModelView {
        id: asteroidModelView1
        anchors.fill: image2
        anchors.topMargin: image2.height * 0.21
        anchors.bottomMargin: image2.height * 0.15
        anchors.leftMargin: image2.width * 0.3
        anchors.rightMargin: image2.width * 0.1

    }

    Image {
        z : -1
        id: image0
        x: 0
        y: 0
        height: parent.height
        width: sourceSize.width * item1.height / sourceSize.height
        source: "qrc:/menu0.png"
    }
    Image {
        z : -1
        id: image1
        anchors.left: image0.right
        anchors.right: image2.left
        height: parent.height
        fillMode: Image.TileHorizontally
        source: "qrc:/menu1.png"
    }
    Image {
        z : -1
        id: image2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        height: parent.height
        width: sourceSize.width * item1.height / sourceSize.height
        source: "qrc:/menu2.png"
    }

}

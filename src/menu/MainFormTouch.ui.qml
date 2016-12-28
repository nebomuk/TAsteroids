import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import "Style.js" as Style



Item {
    id: item1
//    width: 800
//    height: 384


    Item
    {
        id: item2

        anchors.left: image0.left
        anchors.right: image1.right // must be right because image1 (middle) gets stretched
        anchors.top: image0.top
        anchors.bottom: image0.bottom

        anchors.topMargin: image0.height * 0.3
        anchors.bottomMargin: image0.height * 0.3
        anchors.leftMargin: image0.width * 0.3
        anchors.rightMargin: -image2.width * 0.1


        Column
        {
            id : column
            anchors.fill: parent
            spacing: parent.height / 10
            //height: parent.height / 2


            MainMenuRectangle
            {
                height: parent.height / 4
                title1: qsTr("New Game")

                MouseArea
                {
                    onClicked:  playerInput.visible = true, column.visible = false
                    anchors.fill: parent

                }
            }
            MainMenuRectangle
            {
                height: parent.height / 4
                title1: qsTr("High Scores")



                MouseArea
                {
                    onClicked:  highScoreContainer.visible = true, column.visible = false
                    anchors.fill: parent

                }
            }
        }

        Connections
        {
            target: rootWindow
            onClosing : column.visible ? close.accepted = true :
                                         close.accepted = false,
                                         column.visible = true,
                                         highScoreContainer.visible = false,
                                         playerInput.visible =  false
        }


        PlayerInput
        {
            id : playerInput
            anchors.fill: parent
            visible: false

            onFinished:  column.visible = true, playerInput.visible = false
        }

        Item
        {
            id : highScoreContainer
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            width : parent.width *0.9
            height: parent.height *0.9

            visible: false


            HighScoreView2
            {
                id : highscore
                anchors.centerIn : parent


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

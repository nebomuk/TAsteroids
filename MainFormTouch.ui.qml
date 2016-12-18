import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import "Style.js" as Style



Item {
    id: item1
//    width: 800
//    height: 384

    RowLayout
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
//        anchors {

//                 centerIn: parent
//             }




        StackLayout
        {
            id : stack
            height: rootWindow.height / 2
            currentIndex: 0


            Item
            {


                Column
                {
                    id : column

                    anchors.leftMargin: parent.width * 0.1
                    anchors.rightMargin: parent.width * 0.1

                    anchors.verticalCenter: parent.verticalCenter
                    spacing: parent.height / 10
                    anchors.left:  parent.left
                    anchors.right:  parent.right
                    //height: parent.height / 2


                    MainMenuRectangle
                    {
                        height: stack.height / 4
                        title1: qsTr("New Game")

                        MouseArea
                        {
                            onClicked:  stack.currentIndex = 1
                            anchors.fill: parent

                        }
                    }
                    MainMenuRectangle
                    {
                        height: stack.height / 4
                        title1: qsTr("High Scores")



                        MouseArea
                        {
                            onClicked:  stack.currentIndex = 1
                            anchors.fill: parent

                        }
                    }
                }

            }

            HighScoreView2
            {
                id : container

            }

        }


        AsteroidModelView {
            id: asteroidModelView1
            width: rootWindow.height / 2
            height: rootWindow.height / 2
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

        }

    }

    Image {
        id: image0
        x: 0
        y: 0
        height: parent.height
        width: sourceSize.width * item1.height / sourceSize.height
        source: "menu0.png"
    }
    Image {
        z : -1
        id: image1
        anchors.left: image0.right
        anchors.right: image2.left
        height: parent.height
        fillMode: Image.TileHorizontally
        source: "menu1.png"
    }
    Image {
        id: image2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        height: parent.height
        width: sourceSize.width * item1.height / sourceSize.height
        source: "menu2.png"
    }

}

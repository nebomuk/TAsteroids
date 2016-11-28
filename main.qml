import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.LocalStorage 2.0
import QtQuick.Controls 1.4
import my.highscoremodel.singleton 1.0

Window {
    id : rootWindow
    visible: true
    flags: Qt.platform.os != "android" && Qt.platform.os != "ios"  ? Qt.FramelessWindowHint | Qt.ToolTip | Qt.WA_TranslucentBackground : parent.flags
    width: 1020
    height: 567
    title: qsTr("Hello World")
    color: "transparent"
    visibility: Qt.platform.os == "android" || Qt.platform.os == "ios" ?  Window.Maximized : Window.AutomaticVisibility;



    MouseArea {
        id: dragMouseArea

        enabled : !(Qt.platform.os == "android" || Qt.platform.os == "ios" )
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        property variant clickPos: "1,1"

        onPressed: {
            clickPos  = Qt.point(mouse.x,mouse.y)
        }

        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            rootWindow.x += delta.x;
            rootWindow.y += delta.y;
        }

        onClicked: {
                      if (mouse.button == Qt.RightButton)
                      {

                          console.log("right click");
                          ctxMenu.popup();

                      }

                  }
        Menu {
                id: ctxMenu
                MenuItem {
                    text: qsTr("&Quit")
                    onTriggered: rootWindow.close()

                }
            }


    MainForm {
        anchors.fill: parent

//        mouseArea.onClicked: {

//            console.log(qsTr('Clicked on background'));

//           HighScoreModel.savePlayerScore("taik",386);

//        }

    }

    }


}

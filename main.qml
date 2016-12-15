import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.LocalStorage 2.0
import QtQuick.Controls 1.4
import my.highscoremodel.singleton 1.0

Window {
    id : rootWindow
    visible: true
    flags: Qt.platform.os != "android" && Qt.platform.os != "ios"  ? Qt.FramelessWindowHint | Qt.ToolTip | Qt.WA_TranslucentBackground : {}
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

        // context menu
        onPressed: {
            clickPos  = Qt.point(mouse.x,mouse.y)
            if (mouse.button == Qt.RightButton)
            {

                console.log("right click");
                ctxMenu.popup();

            }
        }

        // context menu

        Menu {
                id: ctxMenu
                MenuItem {
                    text: qsTr("&Quit")
                    onTriggered: rootWindow.close()

                }
            }

        // window dragging
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            rootWindow.x += delta.x;
            rootWindow.y += delta.y;
        }




//    MainForm {
//        id : mainForm
//        anchors.fill: parent
//        asteroidModelViewMouseArea.onClicked: asteroidModelView.nextAsteroid()

//    }
        Loader {
            id : myLoader
            readonly property MainForm mainForm: item
            anchors.fill: parent
            source: "MainForm.ui.qml";
        }

        Connections {
                  target: myLoader.item.asteroidModelViewMouseArea
                  onClicked: myLoader.item.asteroidModelView.nextAsteroid()
              }




    }


    onClosing: {
        var stack = mainForm.mainMenu

            if(stack.depth > 1){
                close.accepted = false
                stack.pop();
            }else{
                return;
            }
        }

    Component.onCompleted: {
//            var dateString = new Date().toLocaleDateString();
//            print(dateString);
        }


}

import QtQuick 2.0

ListView {
    id : menuLevel0ListView
    anchors.rightMargin: 0
    anchors.bottomMargin: 0
           anchors.leftMargin: 0
           anchors.topMargin: 0
           z: 1
           anchors.fill: parent
           spacing : 10
           model: ListModel {
               id: listModel1
               ListElement {
                   title: qsTr("New Game")
               }
               ListElement {
                   title: qsTr("High Scores")
               }
               ListElement {
                   title: qsTr("About")
               }
               ListElement {
                   title: qsTr("Exit")
               }
           }

           delegate: Item {

               id: item1
               x: 0
               y: 0
               width: parent.width
               height: 62



               Rectangle {
                   id: rectangle2
                   x: 0
                   y: 0
                   width: parent.width
                   height: 58
                   color: "#00000000"
                   radius: 0
                   border.color: "#0095a6"
                   border.width: 2

                   Text {
                       text: title
                       font.pointSize: 20
                       color: "#01375D"
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter:  parent.horizontalCenter
                   }
               }



               MouseArea {

                      anchors.fill: parent
                      hoverEnabled: true
                      onEntered:  rectangle2.color = rectangle2.border.color
                      onExited: rectangle2.color = "#00000000"


               }



           }

}

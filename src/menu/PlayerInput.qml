import QtQuick 2.2
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import my.highscoremodel.singleton 1.0


Item
{
    id : playerInput
    property int highScore : 0
    signal  finished(); // when enter is pressed

    Text {
        id : placeholder
        color: "#01375D"
        font.pointSize: input.font.pointSize
        text: qsTr("Enter your name");
        font.family: "OCR A Extended"
        horizontalAlignment: Text.AlignLeft
//        anchors.top : parent.top
//        anchors.bottom: input.top
//        anchors.right: parent.right
//        anchors.left: parent.left
        anchors.fill: input
    }
    TextInput
    {
        color: "#0095a6"
        id : input
        anchors.right: parent.right
        anchors.left: parent.left
        onTextChanged :
        {
            placeholder.visible = text.length == 0
        }
        inputMethodHints: Qt.ImhNoPredictiveText
        onAccepted:  {

            finished();
            if(text.length > 0 && highScore > 0)
            {
                // save to database
                HighScoreModel.savePlayerScore(text,highScore);
                text = "";
            }

        }

    }
//        TextField
//        {
//            id : input
//            placeholderText: qsTr("Enter your name");
//            horizontalAlignment: Text.AlignHCenter
//            anchors.right: parent.right
//            anchors.left: parent.left
//            style: TextFieldStyle {
//                    textColor: "#0095a6"
//                    placeholderTextColor : "#01375D"
//                }

//        }
    Rectangle
    {
        anchors.topMargin: input.height * 0.3
        anchors.top : input.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        height: input.height * 0.05
        color: "#0095a6"

    }


     onVisibleChanged: {
         if (visible) {

                           input.forceActiveFocus()
                       }
         else
         {
             Qt.inputMethod.hide()
             input.focus = false
         }
     }


}

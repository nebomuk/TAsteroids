import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Scene3D 2.0
import QtQuick 2.5

Item
{


Scene3D {
            id: scene3d
            anchors.fill:  parent
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
            multisample: Qt.platform.os  != "android" && Qt.platform.os != "ios"


            AsteroidCameraEntity {
                id : asteroidEntity
            }

            property var currentIndex : 0

            property var meshDict : [
//            {
//                scale : 1.5,
//                meshSource : "qrc:/models/golevka.obj"
//            },
//            {
//                scale : 0.3,
//                meshSource : "qrc:/models/toutatis.obj"
//            },
//            {
//                scale : 0.3,
//                meshSource : "qrc:/new_models/detailedVolcano.obj"
//            },
//            {
//                scale : 0.04,
//                meshSource : "qrc:/new_models/alternativeKleopatra.obj"
//            },
//            {
//                scale : 0.4,
//                meshSource : "qrc:/new_models/detailedSpiky.obj"
//            },
            {
                scale : 0.5,
                meshSource : "qrc:/new_models/virus.obj"
            }

            ];


            function nextAsteroid()
            {
                asteroidEntity.meshSource = meshDict[currentIndex].meshSource;
                asteroidEntity.asteroidScale = meshDict[currentIndex].scale;
                if(currentIndex < meshDict.length-1)
                {
                    currentIndex++;

                }
                else
                {
                    currentIndex = 0;
                }

            }




        }

MouseArea
{
    id : asteroidModelViewMouseArea
    anchors.fill: parent
    acceptedButtons: Qt.LeftButton
    onClicked: scene3d.nextAsteroid()
}



}



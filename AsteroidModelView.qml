import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Scene3D 2.0
import QtQuick 2.5


Scene3D {
            id: scene3d
            anchors.margins: 10
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio


            AsteroidCameraEntity {
                id : asteroidEntity
            }

            property var currentIndex : 0

            property var meshDict : [
            {
                scale : 1.2,
                meshSource : "qrc:/models/bacchus.obj"
            },
            {
                scale : 0.006,
                meshSource : "qrc:/models/kleopatra.obj"
            },
            {
                scale : 1.5,
                meshSource : "qrc:/models/golevka.obj"
            },
            {
                scale : 30.0,
                meshSource : "qrc:/models/ky26.obj"
            },
            {
                scale : 0.3,
                meshSource : "qrc:/models/toutatis.obj"
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




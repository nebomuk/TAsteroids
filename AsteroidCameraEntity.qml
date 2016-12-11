import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Scene3D 2.0
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Entity {
    id: root
    objectName: "root"

    property alias meshSource : asteroid.meshSource
    property alias asteroidScale : asteroid.scale

    // Use the renderer configuration specified in ForwardRenderer.qml
    // and render from the mainCamera
    components: [
        RenderSettings {
            activeFrameGraph: SortedForwardRenderer {
                id: renderer
                camera: mainCamera
            }
        },
        // Event Source will be set by the Qt3DQuickWindow
        InputSettings { }
    ]

    BasicCamera {
        id: mainCamera
        position: Qt.vector3d( 0.0, 3.5, 5.0 )
        viewCenter: Qt.vector3d( 0.0, 3.5, 0.0 )
    }


    PhongMaterial {
        id: redAdsMaterial
        ambient: "#002D33"
        diffuse: "#00B9CE"
    }


    Asteroid {
        id: asteroid
        material: redAdsMaterial
        y: 3.5
        scale: 0.3

        ParallelAnimation {
            loops: Animation.Infinite
            running: true

            NumberAnimation {
                target: asteroid
                property: "theta"
                from: 0; to: 360
                duration: 2000
            }

            NumberAnimation {
                target: asteroid
                property: "phi"
                from: 0; to: 360
                duration: 2000
            }
        }
    }
}

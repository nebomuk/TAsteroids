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

    FirstPersonCameraController { camera: mainCamera }


    PhongMaterial {
        id: redAdsMaterial
        ambient: Qt.rgba( 0.02, 0.02, 0.1, 1.0 )
        diffuse: Qt.rgba( 0.0, 0.0, 0.8, 1.0 )
    }


    TrefoilKnot {
        id: trefoilKnot
        material: redAdsMaterial
        y: 3.5
        scale: 0.5

        ParallelAnimation {
            loops: Animation.Infinite
            running: true

            NumberAnimation {
                target: trefoilKnot
                property: "theta"
                from: 0; to: 360
                duration: 2000
            }

            NumberAnimation {
                target: trefoilKnot
                property: "phi"
                from: 0; to: 360
                duration: 2000
            }
        }
    }
}

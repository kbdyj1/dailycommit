import QtQuick 2.0
import QtQuick3D

Item {
    id: root

    width: 640; height: 480

    View3D {
        id: view
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "blue"
            backgroundMode: SceneEnvironment.Color
        }

        Node {
            id: scene

            PerspectiveCamera {
                id: camera
                z: 300
            }
            DirectionalLight {
                z: 400
                brightness: 100
            }
            // should be fixed !!!
            //
            // No GLSL shader code found (versions tried:  QList(120) ) in baked shader
            // QShader(stage=0 shaders=QList(ShaderKey(1 Version(130 QFlags()) 0)) desc.isValid=true)
            // Failed to build graphics pipeline state
            Model {
                source: "#Rectangle"
                materials: DefaultMaterial {
                    diffuseColor: "lightsteelblue"
                }
                PropertyAnimation on eulerRotation.x {
                    loops: Animation.Infinite
                    duration: 5000
                    from: 0
                    to: 360
                }
                PropertyAnimation on eulerRotation.y {
                    loops: Animation.Infinite
                    duration: 5000
                    from: 0
                    to: 360
                }
            }
        }
    }
}

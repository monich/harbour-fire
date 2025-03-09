import QtQuick 1.1
import com.nokia.meego 1.0
import meego.fire 1.0

Window {
    FireItem {
        id: fire

        width: 320
        anchors.centerIn: parent
        height: parent.height * width/parent.width
        scale: parent.width/width // Scaling is far more efficient this way
        intensity: 1
        active: Qt.application.active
    }

    MouseArea {
        property real pressY
        property real pressIntensity

        anchors.fill: parent

        onPressed: {
            pressY = mouseY
            pressIntensity = fire.intensity
        }

        onPositionChanged: fire.intensity = pressIntensity + (pressY - mouseY)/height
    }

    HarbourDisplayBlanking {
        requestInterval: 10
        pauseRequested: Qt.application.active
    }
}

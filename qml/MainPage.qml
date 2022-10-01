import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.fire 1.0

Page {
    id: thisPage

    FireItem {
        id: fire

        width: 320
        anchors.centerIn: parent
        height: thisPage.height * width/thisPage.width
        scale: thisPage.width/width // Scaling is far more efficient this way
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
}

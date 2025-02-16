import QtQuick 2.0
import QtSensors 5.0
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
        wind: Math.max(Math.min(accelerometer.g/10, 1), -1)
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

    Accelerometer {
        id: accelerometer

        active: !fire.idle

        readonly property real g: (reading && active) ?
            (thisPage.orientation == Orientation.Landscape ? -reading.y :
            thisPage.orientation == Orientation.LandscapeInverted ? reading.y :
            thisPage.orientation == Orientation.PortraitInverted ? -reading.x : reading.x) : 0
    }
}

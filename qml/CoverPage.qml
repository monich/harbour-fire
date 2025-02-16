import QtQuick 2.0
import QtSensors 5.0
import Sailfish.Silica 1.0
import harbour.fire 1.0

CoverBackground {
    FireItem {
        id: fire

        anchors.fill: parent
        wind: Math.min(Math.max(accelerometer.x/9, -1), 1)
        intensity: Math.min(Math.max(accelerometer.g/9, -1), 1)
        active: !HarbourSystemState.displayOff && !HarbourSystemState.locked
    }

    Accelerometer {
        id: accelerometer

        readonly property real x: (reading && active) ? reading.x : 0
        readonly property real g: (reading && active) ? Math.sqrt(reading.x * reading.x + reading.y * reading.y) : 0

        active: !fire.idle
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        radius: Theme.paddingMedium
        border {
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2 /* opacityFaint */)
            width: Math.max(Theme.paddingSmall/2, 1)
        }
    }
}

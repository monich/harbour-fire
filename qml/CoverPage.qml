import QtQuick 2.0
import QtSensors 5.0
import Sailfish.Silica 1.0
import harbour.fire 1.0

CoverBackground {
    property bool _pause

    FireItem {
        id: fire

        anchors.fill: parent
        wind: Math.min(Math.max(accelerometer.x/9, -1), 1)
        intensity: Math.min(Math.max(accelerometer.g/9, -1), 1)
        active: !HarbourSystemState.displayOff && !HarbourSystemState.locked && !_pause
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

    Timer {
        id: actionIconTimer

        interval: 1000
    }

    CoverActionList {
        CoverAction {
            iconSource: !actionIconTimer.running ? "" :
                _pause ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: {
                _pause = !_pause
                actionIconTimer.start()
            }
        }
    }
}

import QtQuick 2.0
import QtSensors 5.0
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0
import harbour.fire 1.0

CoverBackground {
    property bool _pause
    readonly property bool _darkOnLight: ('colorScheme' in Theme) && Theme.colorScheme === 1
    readonly property string _actionButtonImageSuffix: _darkOnLight ? ("?" + Theme.overlayBackgroundColor) : ""

    Item {
        anchors {
            fill: parent
            margins: Theme.paddingSmall
        }

        Rectangle {
            id: mask

            anchors.fill: parent
            radius: Theme.paddingSmall
            color: Theme.highlightBackgroundColor
            visible: false
        }

        FireItem {
            id: fire

            visible: false
            anchors.fill: parent
            wind: Math.min(Math.max(accelerometer.x/9, -1), 1)
            intensity: Math.min(Math.max(accelerometer.g/9, -1), 1)
            active: !HarbourSystemState.displayOff && !HarbourSystemState.locked && !_pause
        }

        OpacityMask {
            anchors.fill: fire
            source: fire
            maskSource: mask
        }
    }

    Accelerometer {
        id: accelerometer

        readonly property real x: active ? _x : 0
        readonly property real g: active ? _g : 0

        property real _x
        property real _g

        active: !fire.idle
        onReadingChanged: {
            _x = reading.x
            _g = Math.sqrt(reading.x * reading.x + reading.y * reading.y)
        }
    }

    Timer {
        id: actionIconTimer

        interval: 1000
    }

    CoverActionList {
        CoverAction {
            iconSource: !actionIconTimer.running ? "empty.png" :
                _pause ? "image://theme/icon-cover-pause" + _actionButtonImageSuffix :
                         "image://theme/icon-cover-play" + _actionButtonImageSuffix
            onTriggered: {
                _pause = !_pause
                actionIconTimer.start()
            }
        }
    }
}

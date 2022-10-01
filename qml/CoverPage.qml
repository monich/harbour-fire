import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        anchors {
            centerIn: parent
            verticalCenterOffset: -Theme.paddingLarge
        }
        sourceSize.width: 3 * parent.width / 4
        source: Qt.resolvedUrl("images/fire.svg")
    }
}

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.fire 1.0

ApplicationWindow {
    id: appWindow

    allowedOrientations: Orientation.All

    cover: Component { CoverPage {} }
    initialPage: Component {
        MainPage {
            allowedOrientations: appWindow.allowedOrientations
        }
    }

    HarbourDisplayBlanking {
        pauseRequested: Qt.application.active &&
            (HarbourBattery.batteryState == HarbourBattery.BatteryStateCharging ||
             HarbourBattery.batteryLevel >= 20)
    }
}

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.fire 1.0

ApplicationWindow {
    id: appWindow

    property real intensity

    allowedOrientations: Orientation.All

    cover: Component {
        CoverPage {
            intensity: appWindow.intensity
        }
    }
    initialPage: Component {
        MainPage {
            id: mainPage

            allowedOrientations: appWindow.allowedOrientations

            Binding {
                target: appWindow
                property: "intensity"
                value: mainPage.intensity
            }
        }
    }

    HarbourDisplayBlanking {
        pauseRequested: Qt.application.active &&
            (HarbourBattery.batteryState == HarbourBattery.BatteryStateCharging ||
             HarbourBattery.batteryLevel >= 20)
    }
}

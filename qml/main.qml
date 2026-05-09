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

    Component.onCompleted: {
        // On devices with a cutout, the Page is narrowed in landscape by default,
        // so that the cutout is left outside of the page. That's not what we want.
        // Setting defaultPageCutoutMode to zero allows us to really run full screen,
        // both in portrait and landscape orientations.

        // The defaultPageCutoutMode property (and support for cutout in general)
        // appeared in Sailfish OS 5

        if ('defaultPageCutoutMode' in appWindow) {
            appWindow.defaultPageCutoutMode = 0 // CutoutMode.FullScreen
        }
    }

    HarbourDisplayBlanking {
        pauseRequested: Qt.application.active &&
            (HarbourBattery.batteryState == HarbourBattery.BatteryStateCharging ||
             HarbourBattery.batteryLevel >= 20)
    }
}

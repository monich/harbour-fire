import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow {
    id: appWindow

    allowedOrientations: Orientation.All

    cover: Component { CoverPage {} }
    initialPage: Component {
        MainPage {
            allowedOrientations: appWindow.allowedOrientations
        }
    }
}

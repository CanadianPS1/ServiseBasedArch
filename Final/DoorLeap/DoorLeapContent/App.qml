import QtQuick 2.15
import DoorLeap 1.0

Window {
    width: mainScreen.width
    height: mainScreen.height

    visible: true
    title: "DoorLeap"

    Screen01 {
        id: mainScreen
        anchors.centerIn: parent
    }
}
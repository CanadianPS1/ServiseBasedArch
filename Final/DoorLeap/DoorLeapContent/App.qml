import QtQuick
import DoorLeap

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


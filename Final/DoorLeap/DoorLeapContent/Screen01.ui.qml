

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import DoorLeap

Rectangle {
    id: screen
    width: Constants.width
    height: Constants.height
    color: "#383737"

    border.width: 0

    Rectangle {
        id: signIn
        color: "#7b7b7b"
        radius: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 669
        anchors.rightMargin: 669
        anchors.topMargin: 135
        anchors.bottomMargin: 223
        Text {
            id: logInText
            color: "#ffffff"
            text: qsTr("Log In")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 228
            anchors.rightMargin: 208
            anchors.topMargin: 41
            anchors.bottomMargin: 601
            font.pixelSize: 50
        }

        TextField {
            id: logInUsername
            objectName: "logInUsername"
            color: "#ffffff"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 138
            anchors.rightMargin: 139
            anchors.topMargin: 138
            anchors.bottomMargin: 536
            placeholderText: "Username"
            passwordCharacter: ""
            font.pointSize: 20
        }

        TextField {
            id: lognPassword
            objectName: "logInPassword"
            color: "#ffffff"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 139
            anchors.rightMargin: 138
            anchors.topMargin: 199
            anchors.bottomMargin: 475
            placeholderText: "Password"
            passwordCharacter: ""
            font.pointSize: 20
            echoMode: TextInput.Password
        }

        Button {
            id: logInButton
            objectName: "logInButton"
            text: qsTr("Log In")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 247
            anchors.rightMargin: 248
            anchors.topMargin: 267
            anchors.bottomMargin: 403
            checkable: false
        }

        Text {
            id: logInError
            visible: false
            color: "#ea2994"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 71
            anchors.rightMargin: 69
            anchors.topMargin: 316
            anchors.bottomMargin: 359
            font.pixelSize: 30
        }

        Text {
            id: createAccountText
            x: -460
            color: "#ffffff"
            text: qsTr("Create Account")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 130
            anchors.rightMargin: 110
            anchors.topMargin: 375
            anchors.bottomMargin: 267
            font.pixelSize: 50
        }

        TextField {
            id: createAccountUsername
            color: "#ffffff"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 138
            anchors.rightMargin: 139
            anchors.topMargin: 461
            anchors.bottomMargin: 213
            placeholderText: "Username"
            passwordCharacter: ""
            font.pointSize: 20
        }

        TextField {
            id: createAccountPassword
            color: "#ffffff"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 139
            anchors.rightMargin: 138
            anchors.topMargin: 523
            anchors.bottomMargin: 151
            placeholderText: "Password"
            passwordCharacter: ""
            font.pointSize: 20
            echoMode: TextInput.Password
        }
        Button {
            id: createAccountButton
            visible: true
            text: qsTr("Create")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 235
            anchors.rightMargin: 236
            anchors.topMargin: 585
            anchors.bottomMargin: 85
            checkable: false
            icon.color: "#ffffff"
            highlighted: false
            flat: false
            clip: false
        }

        Text {
            id: createAccountError
            visible: false
            color: "#ea2994"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 71
            anchors.rightMargin: 69
            anchors.topMargin: 643
            anchors.bottomMargin: 32
            font.pixelSize: 30
        }
    }
    states: [
        State {
            name: "clicked"
        }
    ]
}

import QtQuick 2.15
import QtQuick.Controls 6.5
import DoorLeap 1.0

Rectangle {
    id: screen
    objectName: "screen"
    width: Constants.width
    height: Constants.height
    color: "#383737"
    radius: 0
    border.width: 0

    Rectangle {
        id: signIn
        objectName: "signIn"
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
            font.pointSize: 20
        }

        TextField {
            id: logInPassword
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
            onClicked: {
                mainObject.LogIn(logInUsername.text, logInPassword.text)
            }
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
            onClicked: {
                mainObject.CreateAccount(createAccountUsername.text,
                                         createAccountPassword.text)
            }
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

    Rectangle {
        id: background
        objectName: "background"
        color: "#7b7b7b"
        radius: 20
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 231
        anchors.rightMargin: 51
        anchors.topMargin: 190
        anchors.bottomMargin: 59

        Grid {
            id: resterantsGrid
            objectName : "resterantsGrid"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 44
            anchors.rightMargin: 40
            anchors.topMargin: 28
            anchors.bottomMargin: 24
        }
    }

    Rectangle {
        id: deliverysButtonBackground
        objectName: "deliverysButtonBackground"
        opacity: 0.2
        color: "#7b7b7b"
        radius: 20
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 1708
        anchors.topMargin: 423
        anchors.bottomMargin: 568
        Button {
            id: deliverysButton
            objectName : "deliverysButton"
            text: qsTr("Deliverys")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            font.pointSize: 21
            flat: true
        }
    }

    Rectangle {
        id: signoutButtonBackground
        objectName: "signoutButtonBackground"
        opacity: 0.2
        color: "#7b7b7b"
        radius: 40
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 1708
        anchors.topMargin: 856
        anchors.bottomMargin: 135
        Button {
            id: signoutButton
            objectName : "signoutButton"
            text: qsTr("Sign Out")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            font.pointSize: 21
            flat: true
        }
    }

    Rectangle {
        id: cartButtonBackground
        objectName: "cartButtonBackground"
        opacity: 0.2
        color: "#7b7b7b"
        radius: 20
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 1708
        anchors.topMargin: 576
        anchors.bottomMargin: 415
        Button {
            id: cartButton
            objectName : "cartButton"
            text: qsTr("Cart")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            font.pointSize: 21
            flat: true
        }
    }

    Rectangle {
        id: homeButtonBackground
        objectName: "homeButtonBackground"
        opacity: 0.2
        color: "#7b7b7b"
        radius: 40
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 1708
        anchors.topMargin: 54
        anchors.bottomMargin: 937

        Button {
            id: homeButton
            objectName : "homeButton"
            text: qsTr("Home")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            font.pointSize: 21
            flat: true
        }
    }

    Rectangle {
        id: ordersButtonBackground
        objectName: "ordersButtonBackground"
        opacity: 0.2
        color: "#7b7b7b"
        radius: 20
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 1708
        anchors.topMargin: 262
        anchors.bottomMargin: 729
        Button {
            id: ordersButton
            objectName : "ordersButton"
            text: qsTr("Orders")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            font.pointSize: 21
            flat: true
        }
    }

    Rectangle {
        id: searchBarBackground
        objectName: "searchBarBackground"
        opacity: 0.2
        color: "#7b7b7b"
        radius: 50
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 578
        anchors.rightMargin: 473
        anchors.topMargin: 74
        anchors.bottomMargin: 917

        TextField {
            id: searchBar
            objectName : "searchBar"
            color: "#fffbfb"
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            anchors.topMargin: 0
            anchors.bottomMargin: 8
            font.pointSize: 25
            placeholderTextColor: "#92ffffff"
            placeholderText: "Search..."
        }
    }

    Item {
        id: __materialLibrary__
    }
}

/*##^##
Designer {
    D{i:12;invisible:true}D{i:14;invisible:true}D{i:16;invisible:true}D{i:18;invisible:true}
D{i:20;invisible:true}D{i:22;invisible:true}D{i:24;invisible:true}
}
##^##*/


import QtQuick 2.12
import QtQuick.Window 2.12
import com.eagleeyex.EagleEYEX 1.0

Window {
    id: root

    title: "Overlay"

    flags: Qt.Popup | Qt.WA_TranslucentBackground | Qt.WA_TransparentForMouseEvents |
           Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Sheet | Qt.WindowTransparentForInput
    color: "transparent"

    x: overlay.x
    y: overlay.y
    width: overlay.width
    height: overlay.height

    Overlay {
        id: overlay
    }

    Text {
        anchors.centerIn: parent
        text: "eagle eye"
    }

    Aimnet {
        id: aimnet
    }

    Repeater {
        id: rectRepeater

        model: aimnet.model

        delegate: Rectangle {
            x: modelData.x
            y: modelData.y
            width: modelData.width
            height: modelData.height

            color: "transparent"
            border.width: 4
            border.color: "red"
        }
    }
}

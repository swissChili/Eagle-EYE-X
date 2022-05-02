import QtQuick 2.12
import QtQuick.Window 2.12
import com.eagleeyex.EagleEYEX 1.0

Window {
    id: root

    title: "Overlay"

    flags: Qt.Popup | Qt.WA_TranslucentBackground | Qt.WA_TransparentForMouseEvents |
           Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Sheet | Qt.WindowTransparentForInput
    color: "transparent"

    visible: true

    x: overlay.x
    y: overlay.y
    width: overlay.width
    height: overlay.height

    Overlay {
        id: overlay
    }

    Aimnet {
        id: aimnet

        onModelChanged: {
            for (let {headX, headY} of model)
            {
                if (withinReticle(headX, headY))
                {
                    actuator.shootAt(headX, headY);
                }
            }
        }
    }

    function withinReticle(x, y)
    {
        let cx = overlay.width / 2, cy = overlay.height / 2;
        let r = 50;
        return (x > cx - r) && (x < cx + r) && (y > cy - r) && (y < cy + r);
    }

    Actuator {
        id: actuator

        width: overlay.width
        height: overlay.height
    }

    Repeater {
        id: rectRepeater

        model: aimnet.model

        delegate: Item {
            Rectangle {
                x: modelData.x
                y: modelData.y
                width: modelData.width
                height: modelData.height

                color: "transparent"
                border.width: 3
                border.color: "#8BBE91"
                radius: 8
            }

            Rectangle {
                x: modelData.headX - 2
                y: modelData.headY - 2
                color: "yellow"
                width: 4
                height: 4
            }
        }
    }

    Image {
        source: "res/aim-reticle.svg"
        anchors.centerIn: parent
        width: 100
        height: 100
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "red"
        border.width: 3
    }

    Rectangle {
        id: statusBg
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.fill: status
        anchors.margins: -10
        visible: aimnet.statusMessage.length > 0
    }

    Label {
        id: status
        x: 10
        y: 10
        text: aimnet.statusMessage
        color: Style.red
    }
}

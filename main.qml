import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0

Window {
    id: window
    width: 790
    height: 580
    visible: true
    title: qsTr("Eagle EYE X")
    flags: Qt.FramelessWindowHint
    color: "transparent"

    OverlayWindow {
        visible: true
    }

    Item {
        id: dragHeader
        anchors.fill: parent

        DragHandler {
            onActiveChanged: if (active) window.startSystemMove()
        }
    }

    Item {
        id: maskedBg

        anchors.fill: parent

        Rectangle {
            id: bgRect
            color: "#1B311D"
            anchors.fill: parent
        }

        Image {
            id: bgImage
            anchors.fill: parent
            source: "res/BG.png"
            fillMode: Image.PreserveAspectFit
        }


        Blend {
            anchors.fill: bgRect
            source: bgRect
            foregroundSource: bgImage
            mode: "multiply"
        }
    }

    Image {
        id: logo
        x: 256
        y: 89
        width: 279
        height: 64
        source: "res/logo.png"
        antialiasing: true
        fillMode: Image.PreserveAspectFit
    }


    Shadow {
        on: logo
    }

    Image {
        id: logBlock
        x: 312
        y: 161
        width: 223
        height: 113.5
        source: "res/log-block.svg"
        fillMode: Image.PreserveAspectFit
    }

    Shadow {
        on: logBlock
    }

    Image {
        id: goButton
        x: 201
        y: 138
        width: 158
        height: 158
        source: "res/Go.svg"
        fillMode: Image.PreserveAspectFit
    }

    Shadow {
        on: goButton
        hover: true
    }

    Image {
        id: closeButton
        width: 79
        height: 26
        anchors.right: parent.right
        anchors.top: parent.top
        source: "res/close-button.svg"
        anchors.rightMargin: -1
        anchors.topMargin: -1
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            onClicked: window.close()
        }
    }

    Shadow {
        on: closeButton
        hover: true
    }

    Image {
        id: statsBlock
        source: "res/stats-block.svg"
        width: 278
        height: 146
        x: 257
        y: 261
    }

    Shadow {
        on: statsBlock
    }

    Image {
        id: waveformBlock
        source: "res/waveform-block.svg"
        width: 277
        height: 78
        x: 257
        y: 396
    }

    Shadow {
        on: waveformBlock
    }

    Label {
        anchors.centerIn: parent
        text: "GO"
        anchors.verticalCenterOffset: -72
        anchors.horizontalCenterOffset: -114
        color: Style.red
        font.pointSize: Style.titleFontSize
    }

    Image {
        id: overlay
        anchors.fill: parent
        source: "res/overlay.png"
        opacity: 0.411
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}D{i:1;locked:true}D{i:3;locked:true}
}
##^##*/

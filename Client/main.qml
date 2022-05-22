import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

Window {
    id: window
    width: 420
    height: 580
    visible: true
    title: qsTr("Eagle EYE X")

    minimumWidth: 420
    minimumHeight: 580

    Universal.accent: Style.red
    Universal.foreground: Style.foreground
    Universal.background: "#557958"
    Universal.theme: Universal.Dark

    OverlayWindow {
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
            fillMode: Image.PreserveAspectCrop
            scale: 0.75
        }


        Blend {
            anchors.fill: bgRect
            source: bgRect
            foregroundSource: bgImage
            mode: "multiply"
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        anchors.margins: 16

        AiLabel {
            color: Style.red
            text: "AiMNET Options"
            font.pointSize: Style.headingFontSize
            Layout.bottomMargin: 16
        }

        SwitchOption {
            text: "Overlay"
        }

        SwitchOption {
            text: "Aimbot"
        }

        SliderOption {
            text: "Flick time (s)"
            from: 0.05
            to: 0.5
            tooltip: "The time in seconds that AiMNET will take to flick"
        }

        SliderOption {
            text: "Shot delay (s)"
            from: 0.01
            to: 0.2
            tooltip: "The delay after flicking before AiMNET will fire a shot. Sometimes CS:GO isn't fast enough to register very fast shots, so don't send this too low."
        }

        SliderOption {
            text: "Mouse sens"
            from: 0.5
            to: 4
            stepSize: 0.1
            tooltip: "Your mouse sensitivity in CS:GO"
        }

        SliderOption {
            text: "Reticle size"
            from: 20
            to: 150
            stepSize: 1
            tooltip: "The size of the aiming reticle (yellow box) within which AiMNET will automatically flick"
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Image {
        id: overlay
        anchors.fill: parent
        source: "res/overlay.png"
        opacity: 0.2
        fillMode: Image.PreserveAspectCrop
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}D{i:1;locked:true}D{i:3;locked:true}
}
##^##*/

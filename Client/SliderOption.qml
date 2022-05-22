import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout {
    id: root

    property string text: "Slider"
    property alias from: slider.from
    property alias to: slider.to
    property alias value: slider.value
    property alias stepSize: slider.stepSize
    property string tooltip: ""

    AiLabel {
        text: root.text
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }

        ToolTip.delay: 500
        ToolTip.visible: root.tooltip !== "" && mouseArea.containsMouse
        ToolTip.text: root.tooltip
    }

    Slider {
        id: slider

        snapMode: Slider.SnapAlways

        Layout.fillWidth: true
        Layout.maximumWidth: parent.width / 2
        Layout.alignment: Qt.AlignVCenter
    }

    AiLabel {
        Layout.preferredWidth: 50
        text: Math.round(slider.value * 100) / 100
    }
}

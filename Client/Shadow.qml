import QtQuick 2.0
import QtGraphicalEffects 1.0

DropShadow {
    id: root

    required property Item on
    property bool hover: false

    anchors.fill: on
    source: on

    horizontalOffset: 0
    verticalOffset: 8
    radius: mouseArea.containsMouse && hover ? 8 : 4
    samples: 16
    color: mouseArea.containsMouse && hover ? Qt.rgba(0, 0, 0, 0.5) : Qt.rgba(0, 0, 0, 0.25)

    MouseArea {
        id: mouseArea

        propagateComposedEvents: true

        anchors.fill: parent
        enabled: root.hover
        hoverEnabled: true
    }
}

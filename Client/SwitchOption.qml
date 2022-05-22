import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout {
    property string text: "Example"

    AiLabel {
        text: parent.text
        Layout.fillWidth: true
    }

    Switch {
        id: sw
    }

    AiLabel {
        Layout.preferredWidth: 50
        text: sw.position === 1.0 ? "ON" : "OFF"
    }
}

pragma Singleton

import QtQuick 2.0

Item {
    property alias orbitron: orbitronLoader.name
    property color red: "#C83737"
    property int titleFontSize: 24
    property int bodyFontSize: 12

    FontLoader {
        id: orbitronLoader
        source: "res/Orbitron.ttf"
    }
}

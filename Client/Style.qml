pragma Singleton

import QtQuick 2.0

Item {
    property alias orbitron: orbitronLoader.name
    property color foreground: "#d7d7d7"
    property color red: "#C83737"
    property int titleFontSize: 24
    property int headingFontSize: 16
    property int bodyFontSize: 10

    FontLoader {
        id: orbitronLoader
        source: "res/Orbitron.ttf"
    }
}

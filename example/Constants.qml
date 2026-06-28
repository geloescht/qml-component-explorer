/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

pragma Singleton
import QtQuick

QtObject {
    readonly property int width: 1404
    readonly property int height: 1872

    /* Edit this comment to add your custom font */
    readonly property font titleFont: Qt.font({
                                                pointSize: 26,
                                                family: "serif"
                                              })
    readonly property font headlineFont: Qt.font({
                                                    pointSize: 22,
                                                    family: "serif"
                                                 })
    readonly property font subHeadlineFont: Qt.font({
                                                    pointSize: 18,
                                                    family: "sans-serif"
                                                 })
    readonly property font textFont: Qt.font({
                                                    pointSize: 14,
                                                    family: "sans-serif"
                                                 })
    readonly property font codeFont: Qt.font({
                                                 family: "monospace",
                                                 pointSize: 12
                                                 })

    readonly property color backgroundColor: "#FFFFFF"
}

/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels
import net.geloescht.ComponentExplorer

Rectangle {
    id: componentExplorerRoot
    anchors.fill: parent
    color: "white"

    Text {
        id: title
        text: "Explore built-in components"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 100
        font.pointSize: 26
        font.family: "serif"
    }

    Rectangle {
        id: listRect
        border.width: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 100
        anchors.topMargin: 40
        clip: true

        RowLayout {
            id: rowLayout
            anchors.fill: parent
            spacing: -2
            uniformCellSizes: false

            Rectangle {
                id: componentList
                width: 200
                height: 200
                color: "#ffffff"
                border.width: 2
                z: 1
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: filterRect
                    height: 50
                    color: "#ffffff"
                    border.width: 2
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    z: 1

                    RowLayout {
                        id: filterLayout
                        anchors.fill: parent
                        spacing: 5
                        anchors.margins: 5

                        TextField {
                            id: filterInput
                            color: "#000000"
                            verticalAlignment: Text.AlignVCenter
                            Layout.fillWidth: true
                            placeholderText: qsTr("Filter Components...")
                            font.pointSize: 12
                            padding: 5

                            property string acceptedText: ""

                            Connections {
                                target: filterInput
                                function onAccepted() {
                                    filterInput.focus = false
                                }
                            }

                            Connections {
                                target: filterInput
                                function onAccepted() {
                                    filterInput.focus = false;
                                    componentListView.model.setFilter(filterInput.text.split(" "), filterName.filterActive + filterProperty.filterActive + filterEnum.filterActive);
                                }
                            }
                        }

                        Image {
                            id: filterName
                            property int filterActive: TypeList.FilterName

                            width: 32
                            height: 32
                            source: filterActive ? "icons/filter-name.svg" : "icons/no-filter-name.svg"
                            fillMode: Image.PreserveAspectFit
                            visible: !componentListView.currentItem
                            
                            MouseArea {
                                id: filterNameMouseArea
                                anchors.fill: parent
                                Connections {
                                    target: filterNameMouseArea
                                    function onClicked() {
                                        filterName.filterActive = filterName.filterActive ? 0 : TypeList.FilterName
                                    }
                                }
                            }
                        }

                        Image {
                            id: filterProperty
                            width: 32
                            height: 32
                            source: filterActive ? "icons/filter-property.svg" : "icons/no-filter-property.svg"
                            property int filterActive: 0
                            fillMode: Image.PreserveAspectFit
                            visible: !componentListView.currentItem
                            
                            MouseArea {
                                id: filterPropertyMouseArea
                                anchors.fill: parent
                                Connections {
                                    target: filterPropertyMouseArea
                                    function onClicked() {
                                        filterProperty.filterActive = filterProperty.filterActive ? 0 : TypeList.FilterProperties
                                    }
                                }
                            }
                        }

                        Image {
                            id: filterEnum
                            width: 32
                            height: 32
                            source: filterActive ? "icons/filter-enum.svg" : "icons/no-filter-enum.svg"
                            property int filterActive: 0
                            fillMode: Image.PreserveAspectFit
                            visible: !componentListView.currentItem
                            
                            MouseArea {
                                id: filterEnumMouseArea
                                anchors.fill: parent
                                Connections {
                                    target: filterEnumMouseArea
                                    function onClicked() {
                                        filterEnum.filterActive = filterEnum.filterActive ? 0 : TypeList.FilterEnums
                                    }
                                }
                            }
                        }
                    }
                }

                ListView {
                    id: componentListView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: filterRect.bottom
                    anchors.bottom: parent.bottom
                    anchors.margins: 0
                    anchors.topMargin: -2
                    currentIndex: -1
                    headerPositioning: ListView.OverlayHeader

                    model: TypeList

                    header: Rectangle {
                        id: componentListHeaderRect
                        height: 50
                        width: componentList.width
                        color: "#FFFFFF"
                        border.width: 2
                        z: 2

                        RowLayout {
                            id: componentListHeaderLayout
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 20

                            Text {
                                width: 120
                                text: "Class Name"
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                                Layout.maximumWidth: componentList.width
                                font.pointSize: 14
                            }

                            Text {
                                width: 120
                                text: "Properties"
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: 120
                                font.pointSize: 14
                                visible: !componentListView.currentItem
                            }

                            Text {
                                width: 120
                                visible: !componentListView.currentItem
                                text: "Methods"
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: 120
                                font.pointSize: 14
                            }
                        }
                    }

                    delegate: Rectangle {
                        id: componentListItemRect
                        height: filterMatch > 0 ? 50 : 0
                        width: componentList.width
                        color: ListView.isCurrentItem ? "#FF000000" : "#00000000"
                        border.width: 1
                        visible: filterMatch > 0

                        Rectangle {
                            width: 50
                            visible: parent.ListView.isCurrentItem
                            color: "#000000"
                            anchors.left: parent.right
                            anchors.leftMargin: -25
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            topRightRadius: 60
                            bottomRightRadius: 60
                        }

                        RowLayout {
                            id: componentListItemLayout
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 20
                            property var textColor: parent.ListView.isCurrentItem ? "#FFFFFFFF" : "#FF000000"

                            Text {
                                width: 120
                                text: name
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                                Layout.maximumWidth: componentList.width
                                font.pointSize: 14
                                color: parent.textColor
                            }

                            Text {
                                width: 120
                                text: nProperties
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: 120
                                font.pointSize: 14
                                color: parent.textColor
                                visible: !componentListView.currentItem
                            }

                            Text {
                                width: 120
                                visible: !componentListView.currentItem
                                color: parent.textColor
                                text: nMethods
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: 120
                                font.pointSize: 14
                            }
                        }

                        MouseArea {
                            id: componentListItemMouseArea
                            anchors.fill: parent

                            Connections {
                                target: componentListItemMouseArea
                                function onClicked() {
                                    componentListView.currentIndex = index
                                }
                            }
                        }
                    }
                    
                    function getModelProperty(rowIndex, propertyIndex) {
                        return model.data(model.index(rowIndex, 0), propertyIndex);
                    }
                    
                    function getCurrentItemProperty(propertyIndex) {
                        return getModelProperty(currentIndex, propertyIndex);
                    }
                }
            }

            Rectangle {
                id: componentDetailsRect
                width: 600
                height: 200
                visible: componentListView.currentItem != null
                color: "#ffffff"
                border.width: 2
                enabled: true
                Layout.fillHeight: true
                Layout.fillWidth: true

                Image {
                    id: detailsCloseButton
                    width: 32
                    height: 32
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.rightMargin: 20
                    anchors.topMargin: 20
                    source: "icons/close.svg"
                    fillMode: Image.PreserveAspectFit

                    MouseArea {
                        id: detailsCloseButtonMouseArea
                        anchors.fill: parent

                        Connections {
                            target: detailsCloseButtonMouseArea
                            function onClicked() {
                                componentListView.currentIndex = -1
                            }
                        }
                    }
                }

                Flickable {
                    id: detailsScroll
                    anchors.fill: parent
                    anchors.margins: 50
                    pixelAligned: true
                    flickableDirection: Flickable.VerticalFlick
                    contentWidth: contentItem.childrenRect.width
                    contentHeight: contentItem.childrenRect.height

                    Column {
                        id: detailsColumn
                        width: componentDetailsRect.width - 50 * 2
                        spacing: 10

                        Text {
                            id: detailsTitle
                            text: componentListView.getModelProperty(componentListView.currentIndex, TypeList.RoleName)
                            font.pointSize: 22
                            font.family: "serif"
                        }

                        Text {
                            property string superclass: componentListView.getModelProperty(componentListView.currentIndex, TypeList.RoleSuperclassName)
                            visible: superclass != ""
                            text: "inherits " + superclass
                            font.pointSize: 14
                        }

                        Component {
                            id: enumComponent
                            RowLayout {
                                width: detailsColumn.width

                                Item {
                                    width: 10
                                }
                                Text {
                                    text: "enum " + name + " { " + Object.keys(values).map((key) => key + " = " + values[key]). join(", ") + " }"
                                    lineHeight: 1.5
                                    wrapMode: Text.WordWrap
                                    font.family: "Monaspace Xenon"
                                    font.pointSize: 12
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        Text {
                            id: ownEnumListHeadline
                            visible: ownEnumListView.count > 0
                            text: qsTr("Own enumerations")
                            verticalAlignment: Text.AlignBottom
                            lineHeight: 1.2
                            font.pointSize: 18
                        }

                        Repeater {
                            id: ownEnumListView
                            model: componentListView.model.enumList(componentListView.currentIndex, EnumList.OwnEnums)
                            delegate: enumComponent
                        }

                        Text {
                            id: inheritedEnumListHeadline
                            visible: inheritedEnumListView.count > 0
                            text: qsTr("Inherited enumerations")
                            verticalAlignment: Text.AlignBottom
                            lineHeight: 1.2
                            font.pointSize: 18
                        }

                        Repeater {
                            id: inheritedEnumListView
                            model: componentListView.model.enumList(componentListView.currentIndex, EnumList.InheritedEnums)
                            delegate: enumComponent
                        }

                        Component {
                            id: propertyComponent
                            RowLayout {
                                width: detailsColumn.width
                                property var flagString: [ "readable", "writable", "constant", "stored", "enum", "flags", "final", "required", "bindable", "has notify signal" ]
                                function flagsToString(flags) {
                                    return flagString.filter((string, index) => (flags & (1<<index))).join(", ");
                                }

                                Item {
                                    width: 10
                                }
                                Text {
                                    text: type + " " + name
                                    font.family: "Monaspace Xenon"
                                    font.pointSize: 12
                                    Layout.fillWidth: true
                                }

                                Text {
                                    color: "#555555"
                                    text: "[ " + flagsToString(flags) + " ]"
                                    font.pointSize: 14
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }

                        Text {
                            id: ownPropertyListHeadline
                            text: qsTr("Own properties")
                            verticalAlignment: Text.AlignBottom
                            lineHeight: 1.2
                            font.pointSize: 18
                            visible: ownPropertyListView.count > 0
                        }

                        Repeater {
                            id: ownPropertyListView

                            model: componentListView.model.propertyList(componentListView.currentIndex, PropertyList.OwnProperties)
                            delegate: propertyComponent
                        }

                        Text {
                            id: inheritedPropertyListHeadline
                            text: qsTr("Inherited properties")
                            verticalAlignment: Text.AlignBottom
                            lineHeight: 1.2
                            font.pointSize: 18
                            visible: inheritedPropertyListView.count > 0
                        }

                        Repeater {
                            id: inheritedPropertyListView

                            model: componentListView.model.propertyList(componentListView.currentIndex, PropertyList.InheritedProperties)
                            delegate: propertyComponent
                        }

                        Component {
                            id: methodComponent
                            RowLayout {
                                width: detailsColumn.width
                                property var methodTypeString: ["invokable", "signal", "slot", "constructor"]

                                Item {
                                    width: 10
                                }

                                Text {
                                    text: returnType + " " + signature
                                    font.family: "Monaspace Xenon"
                                    font.pointSize: 12
                                    Layout.fillWidth: true
                                }
                                Text {
                                    color: "#555555"
                                    text: "[ " + access + " " + methodTypeString[methodType] + " ]"
                                    horizontalAlignment: Text.AlignRight
                                    font.pointSize: 14
                                }
                            }
                        }
                        
                        Text {
                            id: ownMethodListHeadline
                            text: qsTr("Own methods")
                            verticalAlignment: Text.AlignBottom
                            lineHeight: 1.2
                            font.pointSize: 18
                            visible: ownMethodListView.count > 0
                        }

                        Repeater {
                            id: ownMethodListView

                            model: componentListView.model.methodList(componentListView.currentIndex, MethodList.OwnMethods)
                            delegate: methodComponent
                        }

                        Text {
                            id: methodListHeadline
                            text: qsTr("Inherited methods")
                            verticalAlignment: Text.AlignBottom
                            lineHeight: 1.2
                            font.pointSize: 18
                            visible: inheritedMethodListView.count > 0
                        }

                        Repeater {
                            id: inheritedMethodListView

                            model: componentListView.model.methodList(componentListView.currentIndex, MethodList.InheritedMethods)
                            delegate: methodComponent
                        }
                    }
                }
            }
        }
    }
}

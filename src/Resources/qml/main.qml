import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import rabiche 1.0


ApplicationWindow {
    id: root
    visible: true
    minimumHeight: 700
    minimumWidth: 650
    title: "Rabiche"

    property bool isPortrait: Screen.primaryOrientation === Qt.PortraitOrientation

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Twitter")
            MenuItem {
                text: qsTr("&Add acount")
                onTriggered: {}
            }
        }

        Menu {
            title: qsTr("Tw&eet")
            MenuItem {
                text: qsTr("&New")
                onTriggered: {
                    if (tabAcounts.count > 0)
                        tabAcounts.getTab(tabAcounts.currentIndex).item.showTweetCrtls()
                }
            }
            MenuItem {
                text: qsTr("&Add image")
                onTriggered: {}
            }
        }
    }

    TabView {
        id: tabAcounts
        anchors.fill: parent
    }

    Component.onCompleted: {
        tabAcounts.insertTab(0, "eliasrm87", Qt.createComponent("AccountView.qml"));
    }
}

/*
  The MIT License (MIT)

  Copyright (c) 2016 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.mpw 1.0

Page {

    property bool masterKey: false

    allowedOrientations: Orientation.All

    Connections {
        target: manager

        onGeneratedMasterKey: {
            masterKey = true;
            password.text = "";
            appWindow.password = "";
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        RemorsePopup { id: remorse }

        PullDownMenu {

            MenuItem {
                text: qsTr("Settings")

                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }

            MenuItem {
                id: clearSites
                text: qsTr("Clear sites")
                enabled: sites.count > 0

                onClicked: {
                    remorse.execute(qsTr("Clearing sites"), function() {
                        if (masterKey) {
                            password.text = "";
                            appWindow.password = "";
                        }
                        enabled = false;

                        manager.clearSites();
                    });
                }
            }

            MenuItem {
                id: copy
                text: qsTr("Copy to clipboard")
                enabled: false

                onClicked: {
                    Clipboard.text = appWindow.password;
                }
            }
        }

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin * 2

            PageHeader {
                id: header
            }

            TextField {
                id: siteUrl
                width: parent.width
                inputMethodHints: Qt.ImhUrlCharactersOnly
                placeholderText: qsTr("Site name (e.g. google.com)")
                validator: RegExpValidator { regExp: /^[\w\.-]*$/ }
                enabled: masterKey

                EnterKey.enabled: siteUrl.text.length > 0 && siteCounter.text.length > 0
                EnterKey.onClicked: getPassword()
            }

            Row {
                width: parent.width
                spacing: Theme.paddingMedium

                ComboBox {
                    id: sitePwdType
                    label: qsTr("Type")
                    currentIndex: 1
                    width: parent.width - siteCounter.width
                    enabled: masterKey

                    menu: ContextMenu {
                        MenuItem { text: qsTr("Maximum") }
                        MenuItem { text: qsTr("Long") }
                        MenuItem { text: qsTr("Medium") }
                        MenuItem { text: qsTr("Basic") }
                        MenuItem { text: qsTr("Short") }
                        MenuItem { text: qsTr("PIN") }
                        MenuItem { text: qsTr("Name") }
                        MenuItem { text: qsTr("Phrase") }
                    }

                    onValueChanged: getPassword()
                }

                TextField {
                    id: siteCounter
                    text: "1"
                    width: 200
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: RegExpValidator { regExp: /^[0-9]+$/ }
                    placeholderText: qsTr("Counter")
                    enabled: masterKey

                    EnterKey.enabled: siteUrl.text.length > 0 && siteCounter.text.length > 0
                    EnterKey.onClicked: getPassword()

                }
            }

            Row {
                width: parent.width
                spacing: Theme.paddingMedium

                Text {
                    id: password
                    width: parent.width - hideButton.width
                    color: Theme.secondaryColor
                    horizontalAlignment: TextInput.AlignHCenter
                    wrapMode: Text.Wrap
                    text: qsTr("Tap to set your master password or use the "Settings" page.")
                    font.pixelSize: masterKey ? Theme.fontSizeMedium : Theme.fontSizeExtraSmall
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
                        enabled: !masterKey
                    }
                }

                IconButton {
                    property bool hide: true;

                    id: hideButton
                    icon.source: "image://theme/icon-splus-show-password";
                    width: 150
                    visible: appWindow.password.length > 0
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        if (!hide) {
                            password.text = "********";
                            icon.source = "image://theme/icon-splus-show-password";
                        } else {
                            password.text = appWindow.password;
                            icon.source = "image://theme/icon-splus-hide-password";
                        }
                        hide = !hide;
                    }
                }
            }

            SectionHeader {
               text: qsTr("Sites")
            }

            Repeater {
                id: sites
                model: recentSites
                enabled: masterKey
                delegate: SiteDelegate {}
            }
        }

        VerticalScrollDecorator {}
    }

    function getPassword() {
        if (masterKey) {
            var pwd = manager.getPassword(siteUrl.text, sitePwdType.currentIndex, siteCounter.text);
            password.text = "********";
            copy.enabled = true;
            appWindow.password = pwd;
        }
    }

}

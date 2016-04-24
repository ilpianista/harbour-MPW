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

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {

            MenuItem {
                text: qsTr("Settings")

                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }

            MenuItem {
                text: qsTr("Clear")

                onClicked: clear()
            }

            MenuItem {
                id: copy
                text: qsTr("Copy to clipboard")
                enabled: false

                onClicked: {
                    Clipboard.text = password.text;
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
                id: site
                width: parent.width
                inputMethodHints: Qt.ImhUrlCharactersOnly
                placeholderText: qsTr("Site name (e.g. google.com)")

                EnterKey.enabled: site.text.length > 0 && counter.text.length > 0
                EnterKey.onClicked: getPassword()
            }

            TextField {
                id: counter
                width: parent.width
                text: "1"
                inputMethodHints: Qt.ImhDigitsOnly
                placeholderText: qsTr("Counter")

                EnterKey.enabled: site.text.length > 0 && counter.text.length > 0
                EnterKey.onClicked: getPassword()

            }

            ComboBox {
                id: type
                label: qsTr("Password type")
                currentIndex: 1

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
            }

            Text {
                id: password
                width: parent.width
                color: Theme.secondaryColor
                horizontalAlignment: TextInput.AlignHCenter
                wrapMode: Text.Wrap
            }
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            var isReady = manager.getName() !== "";
            site.enabled = isReady;

            if (!isReady) {
                password.text = qsTr("Please fill your name and master password in the Settings page!");
            } else {
                password.text = "";
            }
        }
    }

    function getPassword() {
        password.text = manager.getPassword(site.text, type.currentIndex, counter.text);
        copy.enabled = true;
    }

    function clear() {
        site.text = "";
        site.forceActiveFocus();
        counter.text = "1"
        type.currentIndex = 1;
        password.text = "";
        copy.enabled = false;
    }
}

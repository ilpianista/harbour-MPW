/*
  The MIT License (MIT)

  Copyright (c) 2016 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distrite, sublicense, and/or sell
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

Page {
    id: page

    allowedOrientations: Orientation.All

    Connections {
        target: manager

        onGeneratedMasterKey: {
            busy.visible = busy.running = false;
            name.enabled = password.enabled = version.enabled = true;
            pageStack.pop();
        }
    }

    Column {
        x: Theme.horizontalPageMargin
        width: parent.width - Theme.horizontalPageMargin * 2
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Settings")
        }

        TextField {
            id: name
            width: parent.width
            text: manager.getName
            placeholderText: qsTr("Full name")

            onTextChanged: {
                if (text.length > 0 && password.text.length > 0) {
                    save.enabled = true;
                }
            }
        }

        TextField {
            id: password
            width: parent.width
            placeholderText: qsTr("Master password")
            echoMode: TextInput.Password

            onTextChanged: {
                if (text.length > 0 && name.text.length > 0) {
                    save.enabled = true;
                }
            }
        }

        ComboBox {
            id: version
            label: qsTr("Algorithm version")
            currentIndex: manager.getAlgorithmVersion()
            width: page.width

            menu: ContextMenu {
                MenuItem { text: qsTr("V0") }
                MenuItem { text: qsTr("V1") }
                MenuItem { text: qsTr("V2") }
                MenuItem { text: qsTr("V3") }
            }
        }

        BusyIndicator {
            id: busy
            visible: false
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: save
            text: qsTr("Generate");
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: false

            onClicked: {
                enabled = name.enabled = password.enabled = version.enabled = false;
                busy.visible = busy.running = true;
                manager.generateMasterKey(name.text, password.text, version.currentIndex);
            }
        }
    }

    Component.onCompleted: {
        name.text = manager.getName();

        if (name.text.length > 0) {
            password.forceActiveFocus();
        }

    }
}

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

    allowedOrientations: Orientation.All

    Column {
        id: column
        x: Theme.horizontalPageMargin
        width: parent.width - Theme.horizontalPageMargin * 2

        PageHeader {
            title: qsTr("Settings")
        }

        TextField {
            id: name
            width: parent.width
            focus: true
            placeholderText: qsTr("Full name")
        }

        TextField {
            id: password
            width: parent.width
            placeholderText: qsTr("Master password")
            echoMode: TextInput.Password
        }

        ComboBox {
            id: version
            label: qsTr("Algorithm version")
            currentIndex: 3

            menu: ContextMenu {
                MenuItem { text: qsTr("V0") }
                MenuItem { text: qsTr("V1") }
                MenuItem { text: qsTr("V2") }
                MenuItem { text: qsTr("V3") }
            }
        }

        Button {
            id: save
            text: qsTr("Save");
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                if (name.text.length > 0 && password.text.length > 0) {
                    manager.generateMasterKey(name.text, password.text, version.currentIndex);
                    pageStack.pop();
                }
            }
        }
    }

    Component.onCompleted: {
        name.text = manager.getName();
    }
}

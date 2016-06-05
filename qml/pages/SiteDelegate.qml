/*
  The MIT License (MIT)

  Copyright (c) 2015 Andrea Scarpino <me@andreascarpino.it>

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

ListItem {
    id: item
    menu: itemMenu

    Label {
        x: Theme.horizontalPageMargin
        font.pixelSize: Theme.fontSizeMedium
        text: site
        wrapMode: Text.Wrap
    }

    ContextMenu {
        id: itemMenu

        MenuItem {
            text: qsTr("Delete")
            onClicked: deleteSite(site)
        }
    }

    onClicked: {
        siteUrl.text = site;
        sitePwdType.currentIndex = typeIndexFromString(type);
        siteCounter.text = counter;

        if (masterKey) {
            password.text = "";
        }
    }

    function typeIndexFromString(type) {
        if (type === "Maximum") {
            return 0;
        } else if (type === "Long") {
            return 1;
        } else if (type === "Minium") {
            return 2;
        } else if (type === "Basic") {
            return 3;
        } else if (type === "Short") {
            return 4;
        } else if (type === "PIN") {
            return 5;
        } else if (type === "Name") {
            return 6;
        } else if (type === "Phrase") {
            return 7;
        }
    }

    function deleteSite(site) {
        remorseAction(qsTr("Deleting"), function() { manager.deleteSite(site) }, 3000);
    }

}

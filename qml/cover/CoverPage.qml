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

CoverBackground {

    property bool hide: true;

    CoverPlaceholder {
        id: placeholder
        text: appWindow.password.length > 0 ? "***" : "MPW"
        icon.source: "/usr/share/icons/hicolor/86x86/apps/harbour-mpw.png"
    }

    CoverActionList {
        id: coverAction
        enabled: appWindow.password.length > 0

        CoverAction {
            iconSource: "image://theme/icon-splus-show-password"

            onTriggered: {
                hide = !hide;
                if (hide) {
                    placeholder.text = "***";
                    iconSource = "image://theme/icon-splus-show-password";
                } else {
                    placeholder.text = appWindow.password;
                    iconSource = "image://theme/icon-splus-hide-password";
                }
            }
        }
    }
}

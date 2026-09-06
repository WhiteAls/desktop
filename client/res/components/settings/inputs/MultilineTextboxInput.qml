// Copyright (c) 2026 Private Internet Access, Inc.
//
// This file is part of the Private Internet Access Desktop Client.
//
// The Private Internet Access Desktop Client is free software: you can
// redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// The Private Internet Access Desktop Client is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Private Internet Access Desktop Client.  If not, see
// <https://www.gnu.org/licenses/>.

import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import "../stores"
import "../../theme"
import "../../common"

FocusScope {
  id: root

  property Setting setting
  property string label: ""
  property string description: ""
  readonly property var currentValue: setting ? setting.currentValue : undefined
  readonly property string text: control.text
  property bool edited: false
  property bool initialized: false
  property bool updatingText: false

  Layout.fillWidth: true
  implicitHeight: content.implicitHeight

  function updateText() {
    if(currentValue === undefined || edited)
      return

    if(control.text !== currentValue) {
      updatingText = true
      control.text = currentValue
      updatingText = false
    }
  }

  function apply() {
    if(!edited || !setting || setting.currentValue === undefined)
      return

    if(setting.currentValue !== control.text)
      setting.currentValue = control.text
    edited = false
  }

  onCurrentValueChanged: updateText()

  ColumnLayout {
    id: content
    anchors.fill: parent
    spacing: 5

    InputLabel {
      Layout.fillWidth: true
      text: root.label
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.preferredHeight: 135
      color: Theme.settings.inputTextboxBackgroundColor
      radius: 3
      border.width: control.activeFocus ? 2 : 1
      border.color: Theme.settings.inputTextboxBorderColor

      ThemedScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: border.width
        label: root.label
        clip: true
        contentWidth: control.width
        contentHeight: control.height

        TextArea {
          id: control
          width: scrollView.availableWidth
          height: Math.max(scrollView.availableHeight,
                           control.contentHeight + control.topPadding +
                           control.bottomPadding)
          padding: 7
          selectByMouse: true
          wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
          color: Theme.settings.inputTextboxTextColor
          font.pixelSize: Theme.settings.inputLabelTextPx
          background: Item {}

          onTextChanged: {
            if(root.initialized && !root.updatingText)
              root.edited = true
          }

          onActiveFocusChanged: {
            if(!control.activeFocus)
              root.apply()
          }
        }
      }
    }

    InputDescription {
      Layout.fillWidth: true
      text: root.description
    }
  }

  Component.onCompleted: {
    updateText()
    initialized = true
  }
}
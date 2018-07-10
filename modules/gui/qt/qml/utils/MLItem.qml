/*****************************************************************************
 * MlItem.qml : Medialibrary item displayed
 ****************************************************************************
 * Copyright (C) 2006-2011 VideoLAN and AUTHORS
 * $Id$
 *
 * Authors: Maël Kervella <dev@maelkervella.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

import QtQuick 2.0
import "qrc:///style/"

Rectangle {
    id: root

    property bool hovered: false
    property bool force_disable: false
    property bool active: !force_disable && (hovered || mouseArea.containsMouse)
    //function active() {
    //    return
    //}

    signal itemClicked


    color: active ? VLCStyle.hoverBgColor : VLCStyle.bgColor

    MouseArea {
        id: mouseArea

        anchors.fill: root

        hoverEnabled: true
        propagateComposedEvents: true
        onClicked: {
            root.itemClicked();
            mouse.accepted = false;
        }
    }
}

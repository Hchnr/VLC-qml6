/*****************************************************************************
 * GridItem.qml : Item displayed inside a grid view
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

MLItem {
    id: root

    property Component cover: Item {}
    property string name: ""
    property string date: ""
    property string infos: ""

    signal playClicked
    signal addToPlaylistClicked

    hovered: name_tooltip.containsMouse

    Column {
        id: column
        x: VLCStyle.margin_xxxsmall
        y: VLCStyle.margin_xxxsmall

        spacing: VLCStyle.margin_xsmall

        /* The full cover component with all added elements */
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: root.width
            height: root.height - info_disp.height - column.spacing

            /* The cover */
            Loader {
                anchors.fill: parent
                sourceComponent: cover
            }

            /* Some infos displayed in the corner of the cover */
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                height: dur_disp.implicitHeight + VLCStyle.margin_xsmall
                width: infos === "" ? 0 : dur_disp.implicitWidth + VLCStyle.margin_xsmall

                color: VLCStyle.bgColor

                Text {
                    id: dur_disp

                    anchors.centerIn: parent

                    text: infos
                    font.pixelSize: VLCStyle.fontSize_small
                    color: VLCStyle.textColor
                }
            }

            Rectangle {
                anchors.fill: parent
                visible: root.active

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: Qt.rgba(VLCStyle.vlc_orange.r,VLCStyle.vlc_orange.g,VLCStyle.vlc_orange.b, 0.6) }
                    GradientStop { position: 1.0; color: VLCStyle.vlc_orange }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: VLCStyle.margin_xsmall
                    spacing: VLCStyle.margin_xsmall

                    /* A addToPlaylist button visible when hovered */
                    Image {
                        height: VLCStyle.icon_normal
                        width: VLCStyle.icon_normal
                        fillMode: Image.PreserveAspectFit

                        visible: root.active
                        source: "qrc:///buttons/playlist/playlist_add.svg"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.addToPlaylistClicked()
                        }
                    }

                    /* A play button visible when hovered */
                    Image {
                        height: VLCStyle.icon_normal
                        width: VLCStyle.icon_normal
                        fillMode: Image.PreserveAspectFit

                        visible: root.active
                        source: "qrc:///toolbar/play_b.svg"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.playClicked()
                        }
                    }
                }
            }
        }

        /* A section with the infos about the album */
        Row {
            id: info_disp

            anchors.horizontalCenter: parent.horizontalCenter
            width: root.width
            height: name_text.height

            layoutDirection: Qt.RightToLeft

            /* The year of the album */
            Text {
                id: date_text

                width: implicitWidth
                height: implicitHeight

                text: date
                font.pixelSize: VLCStyle.fontSize_normal
                color: VLCStyle.textColor
            }

            /* The title of the album elided */
            Text {
                id: name_text

                width: parent.width - date_text.width
                height: implicitHeight

                elide: Text.ElideRight
                font.bold: true
                text: name
                font.pixelSize: VLCStyle.fontSize_normal
                color: VLCStyle.textColor

                ToolTipArea {
                    id: name_tooltip
                    anchors.fill: parent
                    text: name
                    activated: parent.truncated
                }

            }
        }
    }
}

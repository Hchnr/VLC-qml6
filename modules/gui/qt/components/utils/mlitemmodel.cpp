/*****************************************************************************
 * mlitemmodel.cpp : Model for medialibrary item
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

#include "mlitemmodel.hpp"

#include "components/mediacenter/mlitem.hpp"
#include "components/mediacenter/mlalbum.hpp"
#include "components/mediacenter/mlalbumtrack.hpp"


MLItemModel::MLItemModel(const QList<std::shared_ptr<MLItem>> item, QObject *parent):
    QAbstractListModel( parent )
{
    ml_item_list = QList<std::shared_ptr<MLItem>>();
    for (int i=0 ; i<item.count() ; i++)
        ml_item_list.append(item.at(i));
}

MLItemModel::~MLItemModel()
{ }

int MLItemModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return ml_item_list.count();
}

QVariant MLItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    std::shared_ptr<MLItem> ml_item = getItem(index);
    switch (role)
    {
    // Albums
    case GET_ALBUM_ID :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getId() );
        else
            return QVariant();
    }
    case GET_ALBUM_TITLE :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getTitle() );
        else
            return QVariant();
    }
    case GET_ALBUM_RELEASE_YEAR :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getReleaseYear() );
        else
            return QVariant();
    }
    case GET_ALBUM_SHORT_SUMMARY :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getShortSummary() );
        else
            return QVariant();
    }
    case GET_ALBUM_COVER :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getCover() );
        else
            return QVariant();
    }
    case GET_ALBUM_TRACKS :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue<MLItemModel*>( ml_album->getTracks() );
        else
            return QVariant();
    }
    case GET_ALBUM_MAIN_ARTIST :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getArtist() );
        else
            return QVariant();
    }
    case GET_ALBUM_ARTISTS :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getArtists() );
        else
            return QVariant();
    }
    case GET_ALBUM_NB_TRACKS :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getNbTracks() );
        else
            return QVariant();
    }
    case GET_ALBUM_DURATION :
    {
        std::shared_ptr<MLAlbum> ml_album = std::dynamic_pointer_cast<MLAlbum>(ml_item);
        if (ml_album != NULL)
            return QVariant::fromValue( ml_album->getDuration() );
        else
            return QVariant();
    }

    // Tracks
    case GET_TRACK_TITLE :
    {
        std::shared_ptr<MLAlbumTrack> ml_track = std::dynamic_pointer_cast<MLAlbumTrack>(ml_item);
        if (ml_track != nullptr)
            return QVariant::fromValue( ml_track->getTitle() );
        else
            return QVariant();
    }
    case GET_TRACK_COVER :
    {
        std::shared_ptr<MLAlbumTrack> ml_track = std::dynamic_pointer_cast<MLAlbumTrack>(ml_item);
        if (ml_track != nullptr)
            return QVariant::fromValue( ml_track->getCover() );
        else
            return QVariant();
    }
    case GET_TRACK_NUMBER :
    {
        std::shared_ptr<MLAlbumTrack> ml_track = std::dynamic_pointer_cast<MLAlbumTrack>(ml_item);
        if (ml_track != nullptr)
            return QVariant::fromValue( ml_track->getTrackNumber() );
        else
            return QVariant();
    }
    case GET_TRACK_DURATION :
    {
        std::shared_ptr<MLAlbumTrack> ml_track = std::dynamic_pointer_cast<MLAlbumTrack>(ml_item);
        if (ml_track != nullptr)
            return QVariant::fromValue( ml_track->getDuration() );
        else
            return QVariant();
    }

    default :
        return QVariant();
    }

}

QHash<int, QByteArray> MLItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    // Albums
    roles[GET_ALBUM_ID] = "album_id";
    roles[GET_ALBUM_TITLE] = "album_title";
    roles[GET_ALBUM_RELEASE_YEAR] = "album_release_year";
    roles[GET_ALBUM_SHORT_SUMMARY] = "album_shortsummary";
    roles[GET_ALBUM_COVER] = "album_cover";
    roles[GET_ALBUM_TRACKS] = "album_tracks";
    roles[GET_ALBUM_MAIN_ARTIST] = "album_main_artist";
    roles[GET_ALBUM_ARTISTS] = "album_artists";
    roles[GET_ALBUM_NB_TRACKS] = "album_nb_tracks";
    roles[GET_ALBUM_DURATION] = "album_duration";

    // Tracks
    roles[GET_TRACK_TITLE] = "track_title";
    roles[GET_TRACK_COVER] = "track_cover";
    roles[GET_TRACK_NUMBER] = "track_number";
    roles[GET_TRACK_DURATION] = "track_duration";

    return roles;
}

QVariantMap MLItemModel::get(int row) {
    QHash<int,QByteArray> names = roleNames();
    QList<int> keys = names.keys();
    QVariantMap res;
    for (int i=0; i<keys.count(); i++)
    {
        int k = keys[i];
        QByteArray v = names.value(k);
        res[v] = index(row, 0).data(k);
    }
    return res;
}

std::shared_ptr<MLItem> MLItemModel::getItem(const QModelIndex &index) const
{
    int r = index.row();
    if (index.isValid())
        return ml_item_list.at(r);
    else
        return NULL;
}

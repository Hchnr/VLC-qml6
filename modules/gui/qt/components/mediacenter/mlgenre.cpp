/*****************************************************************************
 * mlgenre.cpp : Medialibrary's genre
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

#include <cassert>
#include "mlgenre.hpp"

MLGenre::MLGenre(const vlc_ml_genre_t *_data, QObject *_parent )
    : QObject(_parent)
    , m_id      ( _data->i_id )
    , m_name    ( QString::fromUtf8( _data->psz_name ) )

{
    assert(_data);
}

uint64_t MLGenre::getId() const
{
    return m_id;
}

QString MLGenre::getName() const
{
    return m_name;
}

unsigned int MLGenre::getNbTracks() const
{
    return m_nbTracks;
}

//QString MLGenre::getPresName() const
//{
//    return m_name;
//}
//
//QString MLGenre::getPresImage() const
//{
//    return QString();
//}
//
//QString MLGenre::getPresInfo() const
//{
//    return QString();
//}

//QList<MLAlbumTrack*> MLGenre::getPLTracks() const
//{
//    QList<MLAlbumTrack*> result;
//    std::vector<medialibrary::MediaPtr> t = m_data->tracks()->all();
//    for (int i=0 ; i<t.size() ; i++ )
//        result.append( new MLAlbumTrack( t[i] ) );
//    return result;
//}
//
//QList<std::shared_ptr<MLItem>> MLGenre::getDetailsObjects(medialibrary::SortingCriteria sort, bool desc)
//{
//    QList<std::shared_ptr<MLItem>> result;
//    medialibrary::QueryParameters queryparam{ sort, desc  };
//    std::vector<medialibrary::AlbumPtr> t = m_data->albums(&queryparam)->all();
//    for (int i=0 ; i<t.size() ; i++ )
//        result.append( std::make_shared<MLAlbum>( t[i] ) );
//    return result;
//}

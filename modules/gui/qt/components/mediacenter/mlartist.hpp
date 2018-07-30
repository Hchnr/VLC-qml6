/*****************************************************************************
 * mlartist.hpp : Medialibrary's artist
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

#ifndef MLARTIST_HPP
#define MLARTIST_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <QObject>
#include <QString>
#include <QList>
#include <memory>
#include <vlc_media_library.h>

#include "mlalbum.hpp"
#include "mlitem.hpp"
#include "mlhelper.hpp"

class MLArtist : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint64_t id READ getId CONSTANT)
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QString shortbio READ getShortBio CONSTANT)
    Q_PROPERTY(QString cover READ getCover CONSTANT)
    Q_PROPERTY(unsigned int nbalbums READ getNbAlbums CONSTANT)
    Q_PROPERTY(unsigned int nbtracks READ getNbTracks CONSTANT)

public:
    MLArtist(const vlc_ml_artist_t *_data, QObject *_parent = nullptr);

    uint64_t getId() const;
    QString getName() const;
    QString getShortBio() const;
    QString getCover() const;
    unsigned int getNbAlbums() const;
    unsigned int getNbTracks() const;

    Q_INVOKABLE QString getPresName() const;
    Q_INVOKABLE QString getPresImage() const;
    Q_INVOKABLE QString getPresInfo() const;
    //Q_INVOKABLE QList<MLAlbumTrack *> getPLTracks() const;

private:
    uint64_t m_id;
    QString m_name;
    QString m_shortBio;
    QString m_cover;
    unsigned int m_nbAlbums;
    unsigned int m_nbTracks;
};

#endif

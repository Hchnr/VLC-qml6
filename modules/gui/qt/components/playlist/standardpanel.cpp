/*****************************************************************************
 * standardpanel.cpp : The "standard" playlist panel : just a treeview
 ****************************************************************************
 * Copyright © 2000-2010 VideoLAN
 * $Id$
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "components/playlist/standardpanel.hpp"

#include "components/playlist/mediacenter_model.hpp" /* PLModel */
#include "components/playlist/views.hpp"          /* 3 views */
#include "util/animators.hpp"                     /* PixmapAnimator */
#include "menus.hpp"                              /* Popup */
#include "input_manager.hpp"                      /* THEMIM */
#include "dialogs_provider.hpp"                   /* THEDP */
#include "recents.hpp"                            /* RecentMRL */
#include "dialogs/playlist.hpp"                   /* Playlist Dialog */
#include "dialogs/mediainfo.hpp"                  /* MediaInfoDialog */
#include "util/qt_dirs.hpp"
#include "util/imagehelper.hpp"
#include "components/playlist/plmodel.hpp"

#include <vlc_services_discovery.h>               /* SD_CMD_SEARCH */
#include <vlc_intf_strings.h>                     /* POP_ */

#define SPINNER_SIZE 32
#include <qt5/QtQuickWidgets/QQuickWidget>

#include "components/mediacenter/mcmedialib.hpp"
#include "components/mediacenter/mlqmltypes.hpp"
#include "components/mediacenter/mlalbummodel.hpp"
#include "components/mediacenter/mlartistmodel.hpp"

#define I_NEW_DIR \
    I_DIR_OR_FOLDER( N_("Create Directory"), N_( "Create Folder" ) )
#define I_NEW_DIR_NAME \
    I_DIR_OR_FOLDER( N_( "Enter name for new directory:" ), \
                     N_( "Enter name for new folder:" ) )

#define I_RENAME_DIR \
    I_DIR_OR_FOLDER( N_("Rename Directory"), N_( "Rename Folder" ) )
#define I_RENAME_DIR_NAME \
    I_DIR_OR_FOLDER( N_( "Enter a new name for the directory:" ), \
                     N_( "Enter a new name for the folder:" ) )

#include <QHeaderView>
#include <QMenu>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QStackedLayout>
#include <QSignalMapper>
#include <QSettings>
#include <QStylePainter>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QFont>

#include <assert.h>

#define DROPZONE_SIZE 112

/* local helper */
inline QModelIndex popupIndex( QAbstractItemView *view );

StandardPLPanel::StandardPLPanel( PlaylistWidget *_parent,
                                  intf_thread_t *_p_intf,
                                  playlist_item_t *p_root,
                                  VLCModel *_p_model )
                : QWidget( _parent ),
                  model( _p_model ),
                  p_intf( _p_intf )
{
    mainLayout = new QHBoxLayout( this );
    mainLayout->setSpacing( 0 ); mainLayout->setMargin( 0 );
    setMinimumWidth( 300 );

    mainView    = NULL;

    currentRootIndexPLId  = -1;
    lastActivatedPLItemId     = -1;

    QList<QString> frames;
    frames << ":/util/wait1.svg";
    frames << ":/util/wait2.svg";
    frames << ":/util/wait3.svg";
    frames << ":/util/wait4.svg";
    spinnerAnimation = new PixmapAnimator( this, frames, SPINNER_SIZE, SPINNER_SIZE );

    /* Saved Settings */
    QFont font = QApplication::font();
    font.setPointSize( font.pointSize() + getSettings()->value( "Playlist/zoom", 0 ).toInt() );
    model->setData( QModelIndex(), font, Qt::FontRole );

    showMainView();

    DCONNECT( THEMIM, leafBecameParent( int ),
              this, browseInto( int ) );

    CONNECT( model, currentIndexChanged( const QModelIndex& ),
             this, handleExpansion( const QModelIndex& ) );
    CONNECT( model, rootIndexChanged(), this, browseInto() );

    setRootItem( p_root, false );

    /* Create a Container for the Art Label
       in order to have a beautiful resizing for the selector above it */
    videoOverlay = new VideoOverlay(this);
}

StandardPLPanel::~StandardPLPanel()
{
    getSettings()->beginGroup("Playlist");
    getSettings()->setValue( "zoom",
                model->data( QModelIndex(), Qt::FontRole ).value<QFont>().pointSize()
                - QApplication::font().pointSize() );
    getSettings()->endGroup();
}

void StandardPLPanel::handleExpansion( const QModelIndex& index )
{
    assert( mainView );
    if( currentRootIndexPLId != -1 && currentRootIndexPLId != model->itemId( index.parent() ) )
        browseInto( index.parent() );
}

inline QModelIndex popupIndex( QAbstractItemView *view )
{
    QModelIndexList list = view->selectionModel()->selectedIndexes();
    if ( list.isEmpty() )
        return QModelIndex();
    else
        return list.first();
}

/* Set the root of the new Playlist */
/* This activated by the selector selection */
void StandardPLPanel::setRootItem( playlist_item_t *p_item, bool b )
{
    Q_UNUSED( b );
    model->rebuild( p_item );
}

void StandardPLPanel::browseInto( const QModelIndex &index )
{
    /* When going toward root in LocationBar, scroll to the item
       that was previously as root */
    QModelIndex newIndex = model->indexByPLID(currentRootIndexPLId,0);
    while( newIndex.isValid() && (newIndex.parent() != index) )
        newIndex = newIndex.parent();

    /* Store new rootindexid*/
    currentRootIndexPLId = model->itemId( index );

    model->ensureArtRequested( index );

    emit viewChanged( index );
}

void StandardPLPanel::browseInto()
{
    browseInto( (currentRootIndexPLId != -1) ?
                 model->indexByPLID( currentRootIndexPLId, 0 ) :
                 QModelIndex() );
}

void StandardPLPanel::wheelEvent( QWheelEvent *e )
{
    if( e->modifiers() & Qt::ControlModifier ) {
        int numSteps = e->delta() / 8 / 15;
        if( numSteps > 0)
            increaseZoom();
        else if( numSteps < 0)
            decreaseZoom();
    }
    // Accept this event in order to prevent unwanted volume up/down changes
    e->accept();
}

bool StandardPLPanel::eventFilter ( QObject *, QEvent *  )
{
    return false;
}

void StandardPLPanel::createMainView()
{
    mainView = new QQuickWidget();

    /*************************************************************
     * PROPERTIES
     *************************************************************/

    QQmlContext *rootCtx = mainView->rootContext();

    std::shared_ptr<PLModel> plmodel = std::make_shared<PLModel>(p_intf);
    rootCtx->setContextProperty( "playlist", plmodel.get());

    MCMediaLib *medialib = new MCMediaLib(p_intf, mainView, plmodel);
    rootCtx->setContextProperty( "medialib", medialib );

    qmlRegisterType<ParentType>( "org.videolan.medialib", 0, 1, "ParentType" );
    qmlRegisterType<MLAlbumModel>( "org.videolan.medialib", 0, 1, "MLAlbumModel" );
    qmlRegisterType<MLArtistModel>( "org.videolan.medialib", 0, 1, "MLArtistModel" );

    mainView->setSource( QUrl ( QStringLiteral("qrc:/qml/MainInterface.qml") ) );
    mainView->setResizeMode(QQuickWidget::SizeRootObjectToView);
}

void StandardPLPanel::updateZoom( int i )
{
    QVariant fontdata = model->data( QModelIndex(), Qt::FontRole );
    QFont font = fontdata.value<QFont>();
    font.setPointSize( font.pointSize() + i );
    if ( font.pointSize() < 5 - QApplication::font().pointSize() ) return;
    if ( font.pointSize() > 3 + QApplication::font().pointSize() ) return;
    model->setData( QModelIndex(), font, Qt::FontRole );
}

void StandardPLPanel::showMainView()
{
    if( mainView == NULL )
        createMainView();
    mainLayout->addWidget( mainView );
    browseInto();
}

void StandardPLPanel::browseInto( int i_pl_item_id )
{
    if( i_pl_item_id != lastActivatedPLItemId ) return;

    QModelIndex index = model->indexByPLID( i_pl_item_id, 0 );

    browseInto( index );

    lastActivatedPLItemId = -1;
}

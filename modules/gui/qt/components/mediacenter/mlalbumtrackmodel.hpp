#ifndef MLTRACKMODEL_HPP
#define MLTRACKMODEL_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <QObject>
#include "mlbasemodel.hpp"
#include "mlalbumtrack.hpp"


class MLAlbumTrackModel : public MLBaseModel
{
    Q_OBJECT

public:
    explicit MLAlbumTrackModel(QObject *parent = nullptr);

    virtual ~MLAlbumTrackModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;
    Q_INVOKABLE void fetchMore(const QModelIndex &parent) override;

    Q_PROPERTY( ParentType::ParentTypes parentType READ parentType WRITE setParentType )
    Q_PROPERTY( int64_t parentId READ parentId WRITE setParentId )
    Q_PROPERTY( MCMediaLib* ml READ ml WRITE setMl )

private:
    void clear() override;
    vlc_ml_sorting_criteria_t roleToCriteria(int role) const override;
    const MLAlbumTrack* getItem(const QModelIndex &index) const;

    unsigned int m_total_count;
    bool m_initialized;
    std::vector<std::unique_ptr<MLAlbumTrack>> m_item_list;
};
#endif // MLTRACKMODEL_HPP

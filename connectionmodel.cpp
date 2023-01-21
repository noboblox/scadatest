#include "connectionmodel.hpp"
#include "connectionevent.h"
#include <algorithm>

ConnectionModel::ConnectionModel()
    : QAbstractTableModel(nullptr)
{
}

QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Orientation::Horizontal)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case COL_ACTIVE:
        return QVariant("active");
    case COL_IP:
        return QVariant("ip");
    case COL_PORT:
        return QVariant("port");
    default:
        return QVariant();
    }
}

void ConnectionModel::Update(const ConnectionEvent& arEvent)
{
    Row entry{arEvent.mIp, arEvent.mPort, arEvent.IsActive()};

    auto it = std::find(mConnections.begin(), mConnections.end(), entry);

    if (it == mConnections.end())
    {
        if (!arEvent.IsConnect())
            return;

        beginInsertRows(QModelIndex(), mConnections.size(), mConnections.size());
        mConnections.push_back(entry);
        endInsertRows();
    }
    else
    {
        const int index = std::distance(mConnections.begin(), it);

        if (arEvent.IsDisconnect())
        {
            beginRemoveRows(QModelIndex(), index, index);
            mConnections.erase(it);
            endRemoveRows();
        }
        else
        {
            it->active = arEvent.IsActive();

            const QModelIndex changed = this->createIndex(index, COL_ACTIVE, nullptr);
            emit dataChanged(changed, changed, QList<int> {Qt::DisplayRole});
        }
    }
}

int ConnectionModel::rowCount(const QModelIndex&) const
{
    return mConnections.size();
}

int ConnectionModel::columnCount(const QModelIndex&) const
{
    return COL_end;
}

QVariant ConnectionModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    const Row& data = mConnections[index.row()];

    switch(index.column())
    {
    case COL_ACTIVE:
        return QVariant(data.active ? "yes" : "no");
    case COL_IP:
        return QVariant(data.peer.toString());
    case COL_PORT:
        {
            QString port;
            port.setNum(data.port);
            return QVariant(port);
        }
    default:
        return QVariant();
    }
}


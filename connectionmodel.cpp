#include "connectionmodel.hpp"
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


void ConnectionModel::Add(const Row& arRow)
{
    beginInsertRows(QModelIndex(), mConnections.size(), mConnections.size());
    mConnections.push_back(arRow);
    endInsertRows();
}

void ConnectionModel::SetActivation(const VRTU::Id& arId, bool value)
{
    auto it = std::find_if(mConnections.begin(), mConnections.end(),
                           [&arId](const auto& e) { return e.id == arId; });

    if (it == mConnections.end())
        return;

    it->active = value;

    int row = std::distance(mConnections.begin(), it);
    auto index = createIndex(row, COL_ACTIVE);
    dataChanged(index, index);
}

void ConnectionModel::Remove(const VRTU::Id& arId)
{
    auto it = std::find_if(mConnections.begin(), mConnections.end(),
                           [&arId](const auto& e) { return e.id == arId; });

    if (it == mConnections.end())
        return;

    const auto index = std::distance(mConnections.begin(), it);
    beginRemoveRows(QModelIndex(), index, index);
    mConnections.erase(it);
    endRemoveRows();
}

std::pair<std::string, int> ConnectionModel::getPeerAddress(const VRTU::Id& arId) const
{
    auto it = std::find_if(mConnections.begin(), mConnections.end(),
                           [&arId](const auto& e) { return e.id == arId; });

    if (it == mConnections.end())
        return std::make_pair("", 0);

    return std::make_pair(it->peer, it->port);
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
        return QVariant(data.peer.c_str());
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


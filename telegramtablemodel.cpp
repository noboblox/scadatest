#include "telegramtablemodel.h"
#include <QByteArray>

TelegramTableModel::TelegramTableModel()
    : QAbstractTableModel(nullptr)
{
}

QVariant TelegramTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Orientation::Horizontal)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case COL_FROM_IP:
        return QVariant("from address");
    case COL_FROM_PORT:
        return QVariant("from port");
    case COL_TO_IP:
        return QVariant("to address");
    case COL_TO_PORT:
        return QVariant("to port");
    case COL_DATA:
        return QVariant("raw data");
    default:
        return QVariant();
    }
}

int TelegramTableModel::rowCount(const QModelIndex&) const
{
    return mTelegrams.size();
}

int TelegramTableModel::columnCount(const QModelIndex&) const
{
    return COL_end;
}

QVariant TelegramTableModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    const RowData& data = mTelegrams[index.row()];

    switch(index.column())
    {
    case COL_FROM_IP:
        return QVariant(data.mFromIp);
    case COL_FROM_PORT:
        return QVariant(data.mFromPort);
    case COL_TO_IP:
        return QVariant(data.mToIp);
    case COL_TO_PORT:
        return QVariant(data.mToPort);
    case COL_DATA:
        return QVariant(data.mData);
    default:
        return QVariant();
    }
}

TelegramTableModel::RowData::RowData(const TelegramEvent& arEvent)
    : mFromIp(arEvent.mFromIp.toString()),
      mFromPort(),
      mToIp(arEvent.mToIp.toString()),
      mToPort(),
      mData(arEvent.mData.toHex(' '))
{
    mFromPort.setNum(arEvent.mFromPort);
    mToPort.setNum(arEvent.mToPort);
}

void TelegramTableModel::Add(const TelegramEvent& arEvent)
{
    const int end = mTelegrams.size();

    beginInsertRows(QModelIndex(), end, end);
    mTelegrams.emplace_back(arEvent);
    endInsertRows();
}

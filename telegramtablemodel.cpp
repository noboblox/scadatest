#include "telegramtablemodel.h"
#include <QByteArray>
#include <QDateTime>
#include <QSize>

TelegramTableModel::TelegramTableModel()
    : QAbstractTableModel(nullptr)
{
}

QVariant TelegramTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Orientation::Horizontal)
        return QVariant();

    if (role == Qt::SizeHintRole)
        return QSize(100, 20);

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case COL_SEQUENCE:
        return QVariant("#");
    case COL_RECEIVE_TIME:
        return QVariant("received");
    case COL_LOCAL:
        return QVariant("local");
    case COL_DIRECTION:
        return QVariant("<>");
    case COL_PEER:
        return QVariant("peer");
    case COL_FRAME_TYPE:
        return QVariant("frame");
    case COL_RECV_SEQ:
        return QVariant("r");
    case COL_SEND_SEQ:
        return QVariant("s");
    case COL_DETAILS:
        return QVariant("details");
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

static QString GetDetails(const VRTU::Apdu& data) noexcept
{
    if (data.HasService())
    {
        return QString(VRTU::EnumApduService::Label(data.GetService()));
    }

    return "";
}

QVariant TelegramTableModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    const Row& data = mTelegrams[index.row()];

    switch(index.column())
    {
    case COL_SEQUENCE:
        return QVariant(index.row() + 1);
    case COL_RECEIVE_TIME:
    {
        QDateTime received;
        received.setMSecsSinceEpoch(data.mReceived.count());
        return QVariant(received.toString("HH:mm:ss,zzz"));
    }
    case COL_LOCAL:
        return QVariant(data.mLocal.c_str());
    case COL_DIRECTION:
        return QVariant(data.mReceiveDirection ? "<<" : ">>");
    case COL_PEER:
        return QVariant(data.mPeer.c_str());
    case COL_FRAME_TYPE:
        return QVariant(VRTU::EnumApduType::Label(data.mApdu.GetType()));
    case COL_RECV_SEQ:
        return QVariant(data.mApdu.HasReceiveSequence() ? QString::number(data.mApdu.GetReceiveSequence()) : "");
    case COL_SEND_SEQ:
        return QVariant(data.mApdu.HasSendSequence() ? QString::number(data.mApdu.GetSendSequence()) : "");
    case COL_DETAILS:
        return QVariant(GetDetails(data.mApdu));
    default:
        return QVariant();
    }
}

void TelegramTableModel::Add(const Row& arRow)
{
    const int end = mTelegrams.size();

    beginInsertRows(QModelIndex(), end, end);
    mTelegrams.emplace_back(arRow);
    endInsertRows();
}

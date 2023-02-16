#ifndef TELEGRAMTABLEMODEL_H
#define TELEGRAMTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "vrtu/datamodel/apdu.hpp"

class TelegramTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnId
    {
        COL_SEQUENCE = 0,
        COL_RECEIVE_TIME,
        COL_LOCAL,
        COL_DIRECTION,
        COL_PEER,
        COL_FRAME_TYPE,
        COL_SEND_SEQ,
        COL_RECV_SEQ,
        COL_DETAILS,

        COL_end
    };

    struct Row
    {
        std::chrono::milliseconds mReceived;
        bool mReceiveDirection;
        std::string mLocal;
        std::string mPeer;
        VRTU::Apdu mApdu;
    };


    explicit TelegramTableModel();

    void Add(const Row& arEvent);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    std::vector<Row> mTelegrams;
};

#endif // TELEGRAMTABLEMODEL_H

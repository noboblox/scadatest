#ifndef TELEGRAMTABLEMODEL_H
#define TELEGRAMTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

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
        COL_DATA,

        COL_end
    };

    struct Row
    {
        std::chrono::milliseconds mReceived;
        bool mReceiveDirection;
        std::string mLocal;
        std::string mPeer;
        std::vector<uint8_t> mData;
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

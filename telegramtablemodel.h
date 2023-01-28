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
        COL_FROM_IP = 0,
        COL_FROM_PORT,
        COL_TO_IP,
        COL_TO_PORT,
        COL_DATA,

        COL_end
    };

    struct Row
    {
        std::string mFromIp;
        int mFromPort;
        std::string mToIp;
        int mToPort;
        std::vector<uint8_t> mData;
    };


    explicit TelegramTableModel();

    void Add(const Row& arEvent);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    std::vector<Row> mTelegrams;
};

#endif // TELEGRAMTABLEMODEL_H

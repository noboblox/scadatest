#ifndef TELEGRAMTABLEMODEL_H
#define TELEGRAMTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "telegramevent.h"

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

    explicit TelegramTableModel();

    void Add(const TelegramEvent& arEvent);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    struct RowData
    {
        explicit RowData(const TelegramEvent&);

        QString mFromIp;
        QString mFromPort;
        QString mToIp;
        QString mToPort;
        QString mData;
    };

    std::vector<RowData> mTelegrams;
};

#endif // TELEGRAMTABLEMODEL_H

#ifndef CONNECTIONMODEL_HPP
#define CONNECTIONMODEL_HPP

#include <QAbstractTableModel>
#include <QHostAddress>
#include <vector>
#include "connectionevent.h"


class ConnectionModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        COL_IP,
        COL_PORT,
        COL_ACTIVE,

        COL_end
    };

    explicit ConnectionModel();

    void Update(const ConnectionEvent& arEvent);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    struct Row
    {
        bool operator==(const Row& other) const noexcept {return peer == other.peer && port == other.port; }

        QHostAddress peer;
        int port;
        bool active;
    };

    std::vector<Row> mConnections;
};

#endif // CONNECTIONMODEL_HPP

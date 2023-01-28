#ifndef CONNECTIONMODEL_HPP
#define CONNECTIONMODEL_HPP

#include <QAbstractTableModel>
#include <string>
#include <vector>
#include "vrtu/id.hpp"

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

    struct Row
    {
        VRTU::Id id;
        std::string peer;
        int port;
        bool active;
    };

    explicit ConnectionModel();

    void Add(const Row& arRow);
    void SetActivation(const VRTU::Id& arId, bool value);
    void Remove(const VRTU::Id& arId);

    std::pair<std::string, int> getPeerAddress(const VRTU::Id& arId) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    std::vector<Row> mConnections;
};

#endif // CONNECTIONMODEL_HPP

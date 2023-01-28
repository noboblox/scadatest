#include "id.hpp"
#include <QUuid>

namespace VRTU
{
    static std::string toStringInternal(const QUuid& id)
    {
        return id.toString(QUuid::WithoutBraces).toStdString();
    }

    Id::Id()
        : mId(toStringInternal(QUuid()))
    {
    }

    /* static */ Id Id::createUUID()
    {
        const auto value = QUuid::createUuid();
        return Id(toStringInternal(value));
    }

    std::string Id::toString() const noexcept
    {
        return mId;
    }

    bool Id::isNull() const
    {
        return mId.empty() || mId == toStringInternal(QUuid());
    }
}

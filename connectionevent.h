#ifndef CONNECTIONEVENT_H
#define CONNECTIONEVENT_H

#include <QObject>
#include <QHostAddress>

class ConnectionEvent : public QObject
{
    Q_OBJECT
public:
    enum Type
    {
        CONNECTED,
        DISCONNECTED,
        STARTDT,
        STOPDT
    };

    ConnectionEvent(Type aType, QHostAddress aIp, int aPort)
        : mType(aType),
          mIp(aIp),
          mPort(aPort)
    {
    }

    explicit ConnectionEvent(const ConnectionEvent& arOther)
        : QObject(),
          mType(arOther.mType),
          mIp(arOther.mIp),
          mPort(arOther.mPort)
    {
    }

    bool IsActive() const noexcept { return mType == STARTDT; }
    bool IsPassive() const noexcept { return mType == STOPDT; }
    bool IsDisconnect() const noexcept { return mType == DISCONNECTED; }
    bool IsConnect() const noexcept { return mType == CONNECTED; }

    const Type mType;
    const QHostAddress mIp;
    const int mPort;
};

#endif

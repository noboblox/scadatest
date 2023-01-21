#ifndef TELEGRAMEVENT_H
#define TELEGRAMEVENT_H

#include <QObject>
#include <QtNetwork/QHostAddress>

class TelegramEvent : public QObject
{
    Q_OBJECT
public:
    explicit TelegramEvent(const QHostAddress& arFromIp, int aFromPort,  const QHostAddress& arToIp, int aToPort,  QByteArray&& arData)
        : mFromIp(arFromIp),
          mFromPort(aFromPort),
          mToIp(arToIp),
          mToPort(aToPort),
          mData(std::move(arData))
    {
    }

    explicit TelegramEvent(const TelegramEvent& arOther)
        : QObject(),
          mFromIp(arOther.mFromIp),
          mFromPort(arOther.mFromPort),
          mToIp(arOther.mToIp),
          mToPort(arOther.mToPort),
          mData(arOther.mData)
    {
    }

public:
    const QHostAddress mFromIp;
    const int mFromPort;
    const QHostAddress mToIp;
    const int mToPort;
    const QByteArray mData;
};

#endif // TELEGRAMEVENT_H

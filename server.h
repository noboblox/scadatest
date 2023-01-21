#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <memory>
#include <string>
#include "cs104_slave.h"
#include "connectionevent.h"
#include "telegramevent.h"

namespace IEC104
{
    class Server : public QObject
    {
        Q_OBJECT
    public:
        explicit Server();

        void Start(const std::string& arInterface, int aPort);
        void Stop() noexcept;
        bool IsRunning() const noexcept;

        const std::string& GetOwnAddress() const noexcept {return mLocalAddress; }

    signals:
        void OnTelegramEvent(const TelegramEvent& arEvent);
        void OnConnectionEvent(const ConnectionEvent& arEvent);

    private:
        static void OnTelegramEventInternal(void* parameter, IMasterConnection connection, uint8_t* msg, int msgSize, bool send) noexcept;
        static void OnConnectionEventInternal(void* parameter, IMasterConnection connection, CS104_PeerConnectionEvent event) noexcept;

        std::unique_ptr<sCS104_Slave, void(*)(sCS104_Slave*)> mpSlave;
        std::string mLocalAddress;
        int mPort;
    };
}
#endif
